#pragma once

#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class UProjectColorSaveSettings;

UCLASS()
class PROJECTCOLOR_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

//--Sound classes references start--//
	UPROPERTY(EditDefaultsOnly, Category = "Project Color | Audio")
	TObjectPtr<USoundMix> MasterSoundMix;

	UPROPERTY(EditDefaultsOnly, Category = "Project Color | Audio")
	TObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Project Color | Audio")
	TObjectPtr<USoundClass> MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Project Color | Audio")
	TObjectPtr<USoundClass> SFXSoundClass;
//--Sound classes references end--//

private:
	void ApplyGraphicsSettings(const UProjectColorSaveSettings* GameSettings) const;
	void ApplyAudioSettings(const UProjectColorSaveSettings* GameSettings) const;

//--Audio modifiers function start--//
	void SetMasterVolume(const float Volume) const;
	void SetMusicVolume(const float Volume) const;
	void SetSFXVolume(const float Volume) const;
//--Audio modifiers function end--//
};
