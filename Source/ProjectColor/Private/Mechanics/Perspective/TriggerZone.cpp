#include "Mechanics/Perspective/TriggerZone.h"
#include <Components/SphereComponent.h>
#include "Mechanics/FragmentsLogic/FragmentedObject.h"
#include "Mechanics/Perspective/CameraAlignmentComponent.h"
#include "ProjectColor/ProjectColorCharacter.h"

ATriggerZone::ATriggerZone()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;

    TriggerArea = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Area"));

    // configura trigger
    TriggerArea->SetMobility(EComponentMobility::Static);
    TriggerArea->SetSphereRadius(60.0f);
    TriggerArea->SetGenerateOverlapEvents(true);
    TriggerArea->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    TriggerArea->SetCollisionProfileName(TEXT("Trigger"));
    
    RootComponent = TriggerArea;
    
    TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ATriggerZone::OnOverlapBegin);
    TriggerArea->OnComponentEndOverlap.AddDynamic(this, &ATriggerZone::OnOverlapEnd);
}

void ATriggerZone::BeginPlay()
{
    Super::BeginPlay();

    if (FragmentedObject.IsValid())
    {
        CameraAlignment = FragmentedObject->GetComponentByClass<UCameraAlignmentComponent>();
    }
}

void ATriggerZone::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bPlayerInside && CameraAlignment.IsValid())
    {
        if (PlayerCharacter.IsValid() && PlayerCharacter->IsFocusModeRunning())
        {
            CameraAlignment->CheckCameraAlignment();   
        }
    }
}

void ATriggerZone::OnOverlapBegin(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    // the player has entered the overlapping area
    if (AProjectColorCharacter* EnteredPlayer = Cast<AProjectColorCharacter>(OtherActor))
    {
        PlayerCharacter = EnteredPlayer;
        bPlayerInside = true;
    }
}

void ATriggerZone::OnOverlapEnd(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32)
{
    // the player has exited the overlapping area
    if (ACharacter* ExitingPlayer = Cast<ACharacter>(OtherActor))
    {
        PlayerCharacter = nullptr;
        bPlayerInside = false;
    }
}

void ATriggerZone::SetPointOfViewLocation(const FVector InLocation)
{
    TriggerArea->SetMobility(EComponentMobility::Movable);
    SetActorLocation(InLocation);
    TriggerArea->SetMobility(EComponentMobility::Static);
}
