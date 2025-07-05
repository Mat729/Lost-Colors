#pragma once

#include "GamePageBase.h"
#include "AudioOptionsPage.generated.h"

class UProjectColorSaveSettings;
class USlider;
class AMainMenuHUD;
class UGameButtonBaseWidget;
class USoundClass;

UCLASS()
class PROJECTCOLOR_API UAudioOptionsPage : public UGamePageBase
{
	GENERATED_BODY()

protected:
//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//--UUSerWidget overrides end--//
	
//--Button widgets setup start--//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> GraphicsButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> BackButton;
//--Button widgets setup end--//

//--Slider widgets setup start--// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USlider> GeneralVolumeSlider;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USlider> MusicVolumeSlider;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USlider> EffectsVolumeSlider;
//--Slider widgets setup end--//

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
	UPROPERTY()
	AHUD* HUD = nullptr;

	UPROPERTY()
	UProjectColorSaveSettings* CurrentSettings = nullptr;
	
//--Buttons delegate binding setup start--//
	void SetButtonsBindings();
	void ResetButtonsBindings();
//--Buttons delegate binding setup end--//

//--Settings state functions start--//
	void LoadCurrentSettings();
	void ApplyCurrentSettingsToUIComponents();
	void SaveCurrentSettings();
//--Settings state functions end--//
	
//--Audio modifiers function start--//
	void SetMasterVolume(float Volume);
	void SetMusicVolume(float Volume);
	void SetSFXVolume(float Volume);
//--Audio modifiers function end--//
	
//--Callbacks Setup start--//
	UFUNCTION()
	void OnGraphicsButtonClicked();
	
	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnGeneralVolumeSliderChanged(const float Value);

	UFUNCTION()
	void OnMusicVolumeSliderChanged(const float Value);

	UFUNCTION()
	void OnEffectsVolumeSliderChanged(const float Value);

	UFUNCTION()
	void OnGeneralVolumeSliderReleased();

	UFUNCTION()
	void OnMusicVolumeSliderReleased();

	UFUNCTION()
	void OnEffectsVolumeSliderReleased();
//--Callbacks Setup end--//
};
