#include "Mechanics/FragmentsLogic/FragmentedObject.h"
#include "Mechanics/FragmentsLogic/FragmentComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Mechanics/Perspective/CameraAlignmentComponent.h"
#include "Components/SplineComponent.h"
#include "NiagaraComponent.h"
#include "ProjectColor/ProjectColorGameMode.h"
#include "Kismet/GameplayStatics.h"


AFragmentedObject::AFragmentedObject()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;

	const TObjectPtr<USceneComponent> RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = RootScene;

	FragmentsContainer = CreateDefaultSubobject<USceneComponent>(TEXT("FragmentsContainer"));
	FragmentsContainer->SetupAttachment(RootScene);
	
	CameraAlignmentComponent = CreateDefaultSubobject<UCameraAlignmentComponent>(TEXT("CameraAlignment"));
	CameraAlignmentComponent->SetupAttachment(RootScene);

	MovementPath = CreateDefaultSubobject<USplineComponent>(TEXT("Movement Path"));
	MovementPath->SetupAttachment(RootScene);

	ResolutionVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ResolutionVFX"));
	ResolutionVFX->SetupAttachment(FragmentsContainer);
	
	Identifier = FGuid::NewGuid().ToString();
}

void AFragmentedObject::BeginPlay()
{
	Super::BeginPlay();

	if (CameraAlignmentComponent)
	{
		CameraAlignmentComponent->OnCameraAligned.AddDynamic(this, &AFragmentedObject::OnCorrectView);	
	}

	// salva i figli diretti del container, solo se Fragment Component
	if (!FragmentsContainer) { return; }
	TArray<USceneComponent*> ChildrenComponents = {}; 
	FragmentsContainer->GetChildrenComponents(false,ChildrenComponents);
	
	for (int32 i = 0; i < ChildrenComponents.Num(); i++)
	{
		if (UFragmentComponent* FragmentComponent = Cast<UFragmentComponent>(ChildrenComponents[i]))
		{
			Fragments.Emplace(FragmentComponent);
		}
	}
	
	FragmentsNumber = Fragments.Num();
}

void AFragmentedObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CameraAlignmentComponent)
	{
		CameraAlignmentComponent->OnCameraAligned.RemoveDynamic(this, &AFragmentedObject::OnCorrectView);
	}
	
	Super::EndPlay(EndPlayReason);
}

FGameStateBlock AFragmentedObject::FetchState() const
{
	const FFragmentedObjectState State = {bResolutionStarted};
	return {
		Identifier,
		FInstancedStruct::Make(State)
	};
}

void AFragmentedObject::ApplyState(const FGameStateBlock& Block)
{
	const FFragmentedObjectState& FragmentState = Block.Data.Get<FFragmentedObjectState>();

	bResolutionStarted = FragmentState.bIsResolved;
	
	// the puzzle has not been resolved yet, do nothing.
	if (!FragmentState.bIsResolved) { return; }

	StopFloatingFragments();
	
	PuzzleResolved(false);

	ColorRoomStart();

	UpdateFragmentedObjectVisuals(FragmentState.bIsResolved);
}

void AFragmentedObject::OnFragmentUpdate(const bool bFragmentReady, const bool bWasReady)
{
	if (bFragmentReady == bWasReady) { return; }
	if (bFragmentReady)
	{
		FragmentsReadyNumber++;
	}
	else
	{
		FragmentsReadyNumber--;
	}
}

void AFragmentedObject::OnCorrectView()
{
	if (FragmentsNumber == FragmentsReadyNumber && bResolutionStarted == false)
	{
		StopFloatingFragments();
		ResolutionStart();
		bResolutionStarted = true;
	}
}

// nonostante il nome, questa funzione toglie ANCHE le collisioni alle static mesh delle regioni (sorry)
void AFragmentedObject::StopFloatingFragments()
{
	for (int32 i = 0; i < FragmentsNumber; i++)
	{
		if (Fragments[i] != nullptr)
		{
			Fragments[i]->StopFloating();
		}
	}
}

void AFragmentedObject::MoveFragmentsToOrigin(const float InAlpha)
{
	if (bResolutionStarted == false) { return; }
	
	for (int32 i = 0; i < FragmentsNumber; i++)
	{
		const FTransform SingleFragmentTransform = Fragments[i]->GetComponentTransform();
		if (!ensureMsgf(FragmentsContainer, TEXT ("Fragments Container invalid!"))) { return; }
		const FTransform FragmentsContainerTransform = FragmentsContainer->GetComponentTransform();
		
		const FVector NewLocation = FMath::Lerp(SingleFragmentTransform.GetLocation(), FragmentsContainerTransform.GetLocation(), InAlpha);
		// non è necessario lerpare la rotazione, perché i frammenti prospettivizzati sono spostati e scalati, ma mai ruotati
		const FQuat FragmentRotation = SingleFragmentTransform.GetRotation();
		const FVector NewScale = FMath::Lerp(SingleFragmentTransform.GetScale3D(), FragmentsContainerTransform.GetScale3D(), InAlpha);

		const FTransform NewTransform(FragmentRotation, NewLocation, NewScale);
		Fragments[i]->SetWorldTransform(NewTransform);
	}
}

void AFragmentedObject::MoveObjectToEndLocation(const float InAlpha)
{
	if (!ensureMsgf(MovementPath, TEXT("Movement Path invalid!"))) { return; }
	if (!ensureMsgf(FragmentsContainer, TEXT("Fragments Container invalid!"))) { return; }
	
	const float SplineLength = MovementPath->GetSplineLength();

	const float AlphaMovement = FMath::Lerp(0,SplineLength, InAlpha);

	const FVector EndPathLocation = MovementPath->GetLocationAtDistanceAlongSpline(AlphaMovement, ESplineCoordinateSpace::World);
	const FRotator EndPathRotator = MovementPath->GetRotationAtDistanceAlongSpline(AlphaMovement, ESplineCoordinateSpace::World);
	const FQuat ObjectQuaternion = FQuat::Slerp(FragmentsContainer->GetComponentQuat(),EndPathRotator.Quaternion(), InAlpha);
	const FRotator ObjectRotator = ObjectQuaternion.Rotator();
	const FVector EndPathScale = MovementPath->GetScaleAtDistanceAlongSpline(AlphaMovement);
	const FTransform NewTransform(ObjectRotator,EndPathLocation, EndPathScale);
		
	FragmentsContainer->SetWorldTransform(NewTransform);
}

void AFragmentedObject::PlayPuzzleResolvedSound() const
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PuzzleResolvedSound, GetActorLocation());
}

void AFragmentedObject::PuzzleResolved(bool bShouldSaveGame)
{
	/**
	 * Broadcast event for all listeners. It is mandatory
	 * to preserve the order of the calls between this
	 * event broadcasting and the game mode notification below.
	 * Changing the order could lead to game saving issues.
	 */
	OnPuzzleResolved.Broadcast();

	UpdateFragmentedObjectVisuals(true);
	
	// notify the game mode, for handling the level resolution
	AProjectColorGameMode* GameMode = Cast<AProjectColorGameMode>(GetWorld()->GetAuthGameMode());
	if (!ensureMsgf(GameMode, TEXT("Game Mode invalid!"))) { return; }
	GameMode->FragmentsPuzzleResolved(this, bShouldSaveGame);
}

void AFragmentedObject::UpdateFragmentedObjectVisuals(const bool bIsResolved)
{
	/**
	 * In our use case this may be already production-ready.
	 * Generally speaking, it would be nice to destroy the actor instead,
	 * but we don't want unpredictable/undeterministics side effects.
	 */
	SetActorHiddenInGame(bIsResolved);
}
