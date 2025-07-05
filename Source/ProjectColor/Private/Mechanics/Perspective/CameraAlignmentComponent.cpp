#include "Mechanics/Perspective/CameraAlignmentComponent.h"
#include "Kismet/GameplayStatics.h"

UCameraAlignmentComponent::UCameraAlignmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCameraAlignmentComponent::BeginPlay()
{
	Super::BeginPlay();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (!PlayerController) { return; }
	PlayerCameraManager = MakeWeakObjectPtr(PlayerController->PlayerCameraManager);
	
	// direzione dell'inquadratira corretta, rappresentata dalla direzione dell'arrow
	CorrectForward = GetForwardVector().GetSafeNormal();
}

void UCameraAlignmentComponent::SetCorrectViewDirection(const FRotator InCorrectCameraRotation)
{
	// ruota la freccia e aggiorna il valore del CorrectForward
	SetWorldRotation(InCorrectCameraRotation);
	CorrectForward = GetForwardVector().GetSafeNormal();
}


void UCameraAlignmentComponent::CheckCameraAlignment()
{
	// se non possiamo conoscere l'attuale rotazione della camera, non possiamo verificarne l'allineamento
	if (!PlayerCameraManager.IsValid()) { return; }
	
	const FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
	const FVector CameraForward = CameraRotation.Vector().GetSafeNormal();

	// prodotto scalare tra vettori normalizzati: ottengo un valore tra -1 e 1 (1 vettori identici, 0 perpendicolari, -1 opposti)
	const float DotProduct = FVector::DotProduct(CameraForward, CorrectForward);
	
	// se l'angolo tra i due vettori è maggiore di 90°, la camera non è allineata
	if (DotProduct <= 0.0f) { return; }

	// calcola arco coseno e converte l'angolo da radianti in gradi
	const float AngleDifference = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

#if WITH_EDITORONLY_DATA
	if (bShowDebugMessages && GEngine)
	{
		if (AngleDifference < CameraRotationThreshold)
		{
			GEngine->AddOnScreenDebugMessage(0, 20.f, FColor::Green, "Camera is aligned");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 20.f, FColor::Red, "Camera is NOT aligned");
		}
	}
#endif

	// confrontiamo con tolleranza ammessa, se minore allora la camera e' sufficientemente allineata alla prospettiva richiesta
	if (AngleDifference < CameraRotationThreshold)
	{
		OnCameraAligned.Broadcast();
	}
}
