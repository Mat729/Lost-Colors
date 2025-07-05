#include "UI/Menu/GraphicOptionsPage.h"

#include "UI/Menu/GameButtonBaseWidget.h"
#include "UI/Menu/MainMenuHUD.h"
#include "Components/Slider.h"
#include "Graphics/GraphicsUtils.h"
#include "SaveSystem/ProjectColorSaveSettings.h"
#include "SaveSystem/SaveGameUtils.h"
#include "UI/GameHUD.h"
#include "RenderingThread.h"

void UGraphicOptionsPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	HUD = GetOwningPlayer()->GetHUD();

	LoadCurrentSettings();
	SetButtonsBindings();
}

void UGraphicOptionsPage::NativeDestruct()
{
	ResetButtonsBindings();

	Super::NativeDestruct();
}

void UGraphicOptionsPage::SetButtonsBindings()
{
	//--Buttons delegates setup start--//
	if (IsValid(AudioButton))
	{
		AudioButton->ButtonClickedDelegate.AddDynamic(this, &UGraphicOptionsPage::OnAudioButtonClicked);
	}

	if (IsValid(BackButton))
	{
		BackButton->ButtonClickedDelegate.AddDynamic(this, &UGraphicOptionsPage::OnBackButtonClicked);
	}
	//--Buttons delegates setup end--//

	if (IsValid(BrightnessSlider))
	{
		BrightnessSlider->OnValueChanged.AddDynamic(this, &UGraphicOptionsPage::OnBrightnessSliderChanged);
		BrightnessSlider->OnMouseCaptureEnd.AddDynamic(this, &UGraphicOptionsPage::OnBrightnessSliderReleased);
	}
}

void UGraphicOptionsPage::ResetButtonsBindings()
{
	//--Buttons delegates unbind start--//
	if (IsValid(AudioButton))
	{
		AudioButton->ButtonClickedDelegate.RemoveDynamic(this, &UGraphicOptionsPage::OnAudioButtonClicked);
	}

	if (IsValid(BackButton))
	{
		BackButton->ButtonClickedDelegate.RemoveDynamic(this, &UGraphicOptionsPage::OnBackButtonClicked);
	}
	//--Buttons delegates unbind end--//

	if (IsValid(BrightnessSlider))
	{
		BrightnessSlider->OnValueChanged.RemoveDynamic(this, &UGraphicOptionsPage::OnBrightnessSliderChanged);
		BrightnessSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UGraphicOptionsPage::OnBrightnessSliderReleased);
	}
}

void UGraphicOptionsPage::LoadCurrentSettings()
{
	CurrentSettings = USaveGameUtils::LoadFromSlot<UProjectColorSaveSettings>(ESlotType::Settings);
	if (!CurrentSettings)
	{
		CurrentSettings = UProjectColorSaveSettings::CreateDefault();
	}
	ApplyCurrentSettingsToUIComponents();
}

void UGraphicOptionsPage::ApplyCurrentSettingsToUIComponents()
{
	if (!IsValid(CurrentSettings)) { return; }

	BrightnessSlider->SetValue(CurrentSettings->GraphicsBrightness);

	SetBrightness(CurrentSettings->GraphicsBrightness);
}

void UGraphicOptionsPage::SaveCurrentSettings()
{
	USaveGameUtils::SaveToSlot(ESlotType::Settings, CurrentSettings);
}

void UGraphicOptionsPage::SetBrightness(const float Brightness)
{
	UGraphicsUtils::SetGlobalBrightness(this, Brightness);

	FlushRenderingCommands();
}

void UGraphicOptionsPage::OnAudioButtonClicked()
{
	if (IsValid(HUD))
	{
		if (AMainMenuHUD* MenuHUD = Cast<AMainMenuHUD>(HUD))
		{
			if (MenuHUD)
			{
				MenuHUD->CreateAudioOptionsPage();
			}
		}
		else if (AGameHUD* GameHUD = Cast<AGameHUD>(HUD))
		{
			if (GameHUD)
			{
				GameHUD->CreateAudioOptionsPage();
			}
		}
		
	}
}

void UGraphicOptionsPage::OnBackButtonClicked()
{
	if (IsValid(HUD))
	{
		if (AMainMenuHUD* MenuHUD = Cast<AMainMenuHUD>(HUD))
		{
			if (MenuHUD)
			{
				MenuHUD->CreateMainMenuPage();
			}
		}
		else if (AGameHUD* GameHUD = Cast<AGameHUD>(HUD))
		{
			if (GameHUD)
			{
				GameHUD->OnPause();
			}
		}
	}
}

void UGraphicOptionsPage::OnBrightnessSliderChanged(const float Value)
{
	SetBrightness(Value);
}

void UGraphicOptionsPage::OnBrightnessSliderReleased()
{
	CurrentSettings->GraphicsBrightness = BrightnessSlider->GetValue();
	SaveCurrentSettings();
}
