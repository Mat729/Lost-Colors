#include "UI/Menu/CameraActorMainMenu.h"

#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"

ACameraActorMainMenu::ACameraActorMainMenu()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraMovementPath=CreateDefaultSubobject<USplineComponent>(FName("Camera Movement Path"));
	CameraMovementPath->SetupAttachment(RootComponent);
}

void ACameraActorMainMenu::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraActorMainMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACameraActorMainMenu::CameraMovement(float InAlpha) const
{
	if (!CameraMovementPath) {return;}
	const float PathLength = CameraMovementPath->GetSplineLength();
	const float AlphaMovement = FMath::Lerp(0, PathLength, InAlpha);
	const FVector EndPathLocation = CameraMovementPath->GetLocationAtDistanceAlongSpline(AlphaMovement,ESplineCoordinateSpace::World);
	
	GetCameraComponent()->SetWorldLocation(EndPathLocation);
}


