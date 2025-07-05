#include "Graphics/GlobalPostProcessManager.h"

AGlobalPostProcessManager::AGlobalPostProcessManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGlobalPostProcessManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnPostProcessVolume();
}

void AGlobalPostProcessManager::SpawnPostProcessVolume()
{
	// Ensures we are in a proper context
	if (!GetWorld()) { return; }

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = GlobalPostProcessName;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // ignore physics collision

	APostProcessVolume* PostProcessVolume = GetWorld()->SpawnActor<APostProcessVolume>(
		FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	PostProcessVolume->bUnbound = true;
	PostProcessVolume->Settings.bOverride_AutoExposureMethod = true;
	PostProcessVolume->Settings.AutoExposureMethod = AEM_Manual;
	PostProcessVolume->Settings.bOverride_AutoExposureBias = true;
	PostProcessVolume->Settings.AutoExposureBias = DefaultBrightness;
}
