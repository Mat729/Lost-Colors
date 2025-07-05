#pragma once

#include "GamePageBase.h"
#include "GraphicOptionsPage.generated.h"

class UProjectColorSaveSettings;
class USlider;
class AMainMenuHUD;
class UGameButtonBaseWidget;

UCLASS()
class PROJECTCOLOR_API UGraphicOptionsPage : public UGamePageBase
{
	GENERATED_BODY()

protected:
//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//--UUSerWidget overrides end--//
	
//--Buttons widget setup start--//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> AudioButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> BackButton;
//--Buttons widget setup end--//

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USlider> BrightnessSlider;

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

	void SetBrightness(float Brightness);
	
//--Callbacks Setup start--//
	UFUNCTION()
	void OnAudioButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnBrightnessSliderChanged(const float Value);

	UFUNCTION()
	void OnBrightnessSliderReleased();
//--Callbacks Setup end--//
};
