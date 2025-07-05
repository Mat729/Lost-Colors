#include "UI/Menu/MainMenuGameMode.h"

#include "Graphics/GlobalPostProcessManager.h"
#include "Graphics/GraphicsUtils.h"
#include "SaveSystem/ProjectColorSaveSettings.h"
#include "SaveSystem/SaveGameUtils.h"
#include "Sound/SoundClass.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// spawn the post process manager into the world
	GetWorld()->SpawnActor<AGlobalPostProcessManager>();

	const UProjectColorSaveSettings* GameSettings = USaveGameUtils::LoadFromSlot<UProjectColorSaveSettings>(ESlotType::Settings);
	if (!GameSettings) { return; }

	ApplyGraphicsSettings(GameSettings);
	ApplyAudioSettings(GameSettings);
}

void AMainMenuGameMode::ApplyGraphicsSettings(const UProjectColorSaveSettings* GameSettings) const
{
	UGraphicsUtils::SetGlobalBrightness(this, GameSettings->GraphicsBrightness);
}

void AMainMenuGameMode::ApplyAudioSettings(const UProjectColorSaveSettings* GameSettings) const
{
	SetMasterVolume(GameSettings->AudioGeneralVolume);
	SetMusicVolume(GameSettings->AudioMusicVolume);
	SetSFXVolume(GameSettings->AudioEffectsVolume);
}

void AMainMenuGameMode::SetMasterVolume(const float Volume) const
{
	if (!IsValid(MasterSoundClass)) { return; }
	
	MasterSoundClass->Properties.Volume = Volume;

	if (MasterSoundMix)
	{
		UGameplayStatics::SetBaseSoundMix(this, MasterSoundMix);
	}
}

void AMainMenuGameMode::SetMusicVolume(const float Volume) const
{
	if (!IsValid(MusicSoundClass)) { return; }
	
	MusicSoundClass->Properties.Volume = Volume;

	if (MasterSoundMix)
	{
		UGameplayStatics::SetBaseSoundMix(this, MasterSoundMix);
	}
}

void AMainMenuGameMode::SetSFXVolume(const float Volume) const
{
	if (!IsValid(SFXSoundClass)) { return; }

	SFXSoundClass->Properties.Volume = Volume;

	if (MasterSoundMix)
	{
		UGameplayStatics::SetBaseSoundMix(this, MasterSoundMix);
	}
}