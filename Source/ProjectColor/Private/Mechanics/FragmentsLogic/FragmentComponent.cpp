#include "Mechanics/FragmentsLogic/FragmentComponent.h"
#include "Mechanics/FragmentsLogic/FragmentedObject.h"

UFragmentComponent::UFragmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;
}

void UFragmentComponent::BeginPlay()
{
	Super::BeginPlay();

	RegionsNumber = GetNumChildrenComponents();
	FragmentedObject = MakeWeakObjectPtr(Cast<AFragmentedObject>(GetOwner()));

	StartFloatingAfterDelay();
}

void UFragmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bFloating)
	{
		UpdateFloating(DeltaTime);
	}
}

// regions
void UFragmentComponent::OnRegionColorChanged(const bool bIsProperlyColored, const bool bWasProperlyColored)
{
	// se lo stato della regione non è cambiato, non fare niente
	if (bIsProperlyColored == bWasProperlyColored) { return; }

	bWasReady = RegionsNumber == RegionsReadyNumber;
	RegionsReadyNumber += bIsProperlyColored ? 1 : -1;
	
	OnRegionUpdate();
}

void UFragmentComponent::OnRegionUpdate() const
{
	if (!ensureMsgf(FragmentedObject.IsValid(), TEXT("Fragmented Object not valid"))){return;}

	const bool bFragmentIsReady = RegionsNumber == RegionsReadyNumber;
	FragmentedObject->OnFragmentUpdate(bFragmentIsReady, bWasReady);
}
// regions

// floating
void UFragmentComponent::StartFloatingAfterDelay()
{
	const float Delay = FMath::RandRange(0.0f, FloatingTime);
	if (!FragmentedObject.IsValid()) { return; }
	FTimerHandle TimerHandle;
	FragmentedObject->GetWorldTimerManager().SetTimer(TimerHandle, this, &UFragmentComponent::StartFloating, Delay, false);
}

void UFragmentComponent::StartFloating()
{
	InitialLocation = GetComponentLocation();
	MinZ = InitialLocation.Z - (FloatingAmplitude * 0.5f);
	MaxZ = InitialLocation.Z + (FloatingAmplitude * 0.5f);
	
	Speed = 1 / FMath::Max(FloatingTime, KINDA_SMALL_NUMBER);
	Alpha = 0.5f;
	
	bFloating = true;
}

void UFragmentComponent::UpdateFloating(const float InDeltaTime)
{
	Alpha = FMath::Min((Alpha + Speed * InDeltaTime), 1.0f);

	const float NewZ = FMath::InterpSinInOut(MinZ, MaxZ, Alpha);
	SetWorldLocation(FVector(InitialLocation.X, InitialLocation.Y, NewZ));

	if (Alpha == 1.0f)
	{
		const float Temp = MinZ;
		MinZ = MaxZ;
		MaxZ = Temp;
		Alpha = 0;
	}
}

void UFragmentComponent::StopFloating()
{
	bFloating = false;

	TArray<USceneComponent*> Regions = {};
	GetChildrenComponents(false,Regions);
	for (USceneComponent* Region : Regions)
	{
		if (UStaticMeshComponent* RegionMesh = Cast<UStaticMeshComponent>(Region))
		{
			RegionMesh->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}
}
// floating
