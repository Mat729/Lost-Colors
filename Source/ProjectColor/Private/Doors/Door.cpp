#include "Doors/Door.h"
#include "Mechanics/FragmentsLogic/FragmentedObject.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootScene->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootScene);
	StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	StaticMeshComponent->CanCharacterStepUpOn = ECB_No;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetEnableGravity(false);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (!ensureMsgf(FragmentedObject, TEXT("Fragmented object not configured"))) { return; }

	FragmentedObject->OnPuzzleResolved.AddDynamic(this, &ADoor::OnFragmentPuzzleResolved);
}

void ADoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (FragmentedObject && FragmentedObject->OnPuzzleResolved.IsAlreadyBound(this, &ADoor::OnFragmentPuzzleResolved))
	{
		FragmentedObject->OnPuzzleResolved.RemoveDynamic(this, &ADoor::OnFragmentPuzzleResolved);
	}
	
	Super::EndPlay(EndPlayReason);
}

void ADoor::OnFragmentPuzzleResolved()
{
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	
	OpenDoor();
}
