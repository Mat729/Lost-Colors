#include "Graphics/GraphicsUtils.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Graphics/GlobalPostProcessManager.h"
#include "Kismet/GameplayStatics.h"

APostProcessVolume* UGraphicsUtils::FindGlobalPostProcessVolume(const UObject* WorldContextObject)
{
	if (!WorldContextObject) { return nullptr; }

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) { return nullptr; }

	for (TActorIterator<APostProcessVolume> It(World); It; ++It)
	{
		if (It->GetName().Equals(AGlobalPostProcessManager::GlobalPostProcessName.ToString()))
		{
			return *It;
		}
	}

	return nullptr;
}

void UGraphicsUtils::SetGlobalBrightness(const UObject* WorldContextObject, const float Brightness)
{
	APostProcessVolume* PostProcessVolume = FindGlobalPostProcessVolume(WorldContextObject);
	if (!PostProcessVolume) return;

	// Overrides every time configs below, because they are mandatory
	PostProcessVolume->Settings.bOverride_AutoExposureMethod = true;
	PostProcessVolume->Settings.AutoExposureMethod = AEM_Manual;
	PostProcessVolume->Settings.bOverride_AutoExposureBias = true;

	// Updates the exposure bias (aka. Brightness)
	PostProcessVolume->Settings.AutoExposureBias = Brightness;

	UpdateCameraPostProcessSettings(WorldContextObject, Brightness);

	UE_LOG(LogTemp, Log, TEXT("Changed brightness to: %f"), Brightness);
}

float UGraphicsUtils::GetGlobalBrightness(const UObject* WorldContextObject)
{
	const APostProcessVolume* PostProcessVolume = FindGlobalPostProcessVolume(WorldContextObject);
	if (!PostProcessVolume) { return 0.0f; }

	// Returns the exposure bias (aka. Brightness)
	return PostProcessVolume->Settings.AutoExposureBias;
}

void UGraphicsUtils::UpdateCameraPostProcessSettings(const UObject* WorldContextObject, const float AutoExposureBias)
{
	if (!WorldContextObject) { return; }
	
	const APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject->GetWorld(), 0);
	if (!PC || !PC->PlayerCameraManager) { return; }
	
	const APawn* Pawn = PC->GetPawn();
	if (!Pawn) { return; }
	
	UCameraComponent* Cam = Pawn->FindComponentByClass<UCameraComponent>();
	if (Cam)
	{
		Cam->PostProcessSettings.bOverride_AutoExposureBias = true;
		Cam->PostProcessSettings.AutoExposureMethod = AEM_Manual;
		Cam->PostProcessSettings.AutoExposureBias = AutoExposureBias;
	}
}
