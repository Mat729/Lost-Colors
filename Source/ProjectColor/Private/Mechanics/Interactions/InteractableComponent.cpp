#include "Mechanics/Interactions/InteractableComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Mechanics/Interactions/InteractionObserverComponent.h"
#include "ProjectColor/ProjectColorCharacter.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("Trigger"));
}

void UInteractableComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// register overlap events
	OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnInteractionAreaBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnInteractionAreaEndOverlap);
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = MakeWeakObjectPtr(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UInteractableComponent::UninitializeComponent()
{
	// remove overlap events registration
	OnComponentBeginOverlap.RemoveDynamic(this, &UInteractableComponent::OnInteractionAreaBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &UInteractableComponent::OnInteractionAreaEndOverlap);

	Super::UninitializeComponent();
}

FText UInteractableComponent::GetInteractionHint()
{
	const FText KeyText = FText::FromName(InteractionBtnKey.GetFName());
	return FText::Format(NSLOCTEXT("Game", "InteractObject", "Press {0} to interact"), KeyText);
}

void UInteractableComponent::OnInteractionAreaBeginOverlap(UPrimitiveComponent*, AActor* OtherActor,
                                                           UPrimitiveComponent*, int32,
                                                           bool, const FHitResult&)
{
	AProjectColorCharacter* Character = Cast<AProjectColorCharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}
	
	InteractingPlayer = MakeWeakObjectPtr(Character);
	bCanInteract = true;

	if (PlayerController.IsValid())
	{
		GetOwner()->EnableInput(PlayerController.Get());
		if (!bInputKeyBound)
		{
			PlayerController->InputComponent->BindKey(InteractionBtnKey, IE_Released, this,
													  &UInteractableComponent::OnInteractionStarted);
			bInputKeyBound = true;
		}	
	}
	
	// notify observer, if any
	NotifyInteractionObserver(this);
}

void UInteractableComponent::OnInteractionAreaEndOverlap(UPrimitiveComponent*, AActor* OtherActor,
                                                         UPrimitiveComponent*, int32)
{
	// ensure that was the player exiting the area
	if (!IsValid(Cast<AProjectColorCharacter>(OtherActor)))
	{
		return;
	}
	
	InteractingPlayer.Reset();
	bCanInteract = false;

	if (PlayerController.IsValid())
	{
		GetOwner()->DisableInput(PlayerController.Get());
	}
	
	// notify observer, if any
	NotifyInteractionObserver(nullptr);
}

void UInteractableComponent::OnInteractionStarted()
{
	if (!InteractingPlayer.IsValid() || !bCanInteract) return;

	Interact(InteractingPlayer.Get());
}

void UInteractableComponent::Interact(AProjectColorCharacter* Interactor)
{
	if (PlayerController.IsValid())
	{
		// during the interaction, the player movements are locked
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);
	}

	// resetta condizioni necessarie per iniziare l'interazione, in modo che non si possa dare inizio a ulteriori interazioni
	InteractingPlayer.Reset();
	bCanInteract = false;
	
	// wipe out the interactable object on the observer after interaction started
	NotifyInteractionObserver(nullptr);
}

void UInteractableComponent::NotifyInteractionObserver(UInteractableComponent* InteractableComponent) const
{
	const AProjectColorCharacter* PlayerCharacter =
		Cast<AProjectColorCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (IsValid(PlayerCharacter))
	{
		UInteractionObserverComponent* ObserverComponent =
			PlayerCharacter->FindComponentByClass<UInteractionObserverComponent>();
		if (IsValid(ObserverComponent))
		{
			ObserverComponent->UpdateCurrentInteractableComponent(InteractableComponent);
		}
	}
}

void UInteractableComponent::OnInteractionCompleted() const
{
	if (PlayerController.IsValid())
	{
		// after the interaction, the player movements are freed
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);
	}
	
	// emit on interaction complete event
	OnInteractionComplete.Broadcast();
}

void UInteractableComponent::EnableInteraction()
{
	if (PlayerController.IsValid())
	{
		GetOwner()->EnableInput(PlayerController.Get());
	}

	UPrimitiveComponent::SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void UInteractableComponent::DisableInteraction()
{
	if (PlayerController.IsValid())
	{
		GetOwner()->DisableInput(PlayerController.Get());
	}

	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"));
}
