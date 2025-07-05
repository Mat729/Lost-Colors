#include "UI/Menu/AudioOptionsPage.h"

#include "UI/Menu/GameButtonBaseWidget.h"
#include "UI/Menu/MainMenuHUD.h"
#include "Components/Slider.h"
#include "SaveSystem/ProjectColorSaveSettings.h"
#include "SaveSystem/SaveGameUtils.h"
#include "Sound/SoundClass.h"
#include "UI/GameHUD.h"

void UAudioOptionsPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	HUD = GetOwningPlayer()->GetHUD();

	LoadCurrentSettings();
	SetButtonsBindings();
}

void UAudioOptionsPage::NativeDestruct()
{
	ResetButtonsBindings();

	Super::NativeDestruct();
}

void UAudioOptionsPage::SetButtonsBindings()
{
//--Buttons delegates setup start--//
	if (IsValid(GraphicsButton))
	{
		GraphicsButton->ButtonClickedDelegate.AddDynamic(this, &UAudioOptionsPage::OnGraphicsButtonClicked);
	}

	if (IsValid(BackButton))
	{
		BackButton->ButtonClickedDelegate.AddDynamic(this, &UAudioOptionsPage::OnBackButtonClicked);
	}
//--Buttons delegates setup end--//

//--Settings delegates setup start--//
	if (IsValid(GeneralVolumeSlider))
	{
		GeneralVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsPage::OnGeneralVolumeSliderChanged);
		GeneralVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UAudioOptionsPage::OnGeneralVolumeSliderReleased);
	}

	if (IsValid(MusicVolumeSlider))
	{
		MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsPage::OnMusicVolumeSliderChanged);
		MusicVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UAudioOptionsPage::OnMusicVolumeSliderReleased);
	}

	if (IsValid(EffectsVolumeSlider))
	{
		EffectsVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsPage::OnEffectsVolumeSliderChanged);
		EffectsVolumeSlider->OnMouseCaptureEnd.AddDynamic(this, &UAudioOptionsPage::OnEffectsVolumeSliderReleased);
	}
//--Settings delegates setup end--//
}

void UAudioOptionsPage::ResetButtonsBindings()
{
//--Buttons delegates unbind start--//
	if (IsValid(GraphicsButton))
	{
		GraphicsButton->ButtonClickedDelegate.RemoveDynamic(this, &UAudioOptionsPage::OnGraphicsButtonClicked);
	}

	if (IsValid(BackButton))
	{
		BackButton->ButtonClickedDelegate.RemoveDynamic(this, &UAudioOptionsPage::OnBackButtonClicked);
	}
//--Buttons delegates unbind end--//

//--Settings delegates unbind start--//
	if (IsValid(GeneralVolumeSlider))
	{
		GeneralVolumeSlider->OnValueChanged.RemoveDynamic(this, &UAudioOptionsPage::OnGeneralVolumeSliderChanged);
		GeneralVolumeSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UAudioOptionsPage::OnGeneralVolumeSliderReleased);
	}

	if (IsValid(MusicVolumeSlider))
	{
		MusicVolumeSlider->OnValueChanged.RemoveDynamic(this, &UAudioOptionsPage::OnMusicVolumeSliderChanged);
		MusicVolumeSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UAudioOptionsPage::OnMusicVolumeSliderReleased);
	}

	if (IsValid(EffectsVolumeSlider))
	{
		EffectsVolumeSlider->OnValueChanged.RemoveDynamic(this, &UAudioOptionsPage::OnEffectsVolumeSliderChanged);
		EffectsVolumeSlider->OnMouseCaptureEnd.RemoveDynamic(this, &UAudioOptionsPage::OnEffectsVolumeSliderReleased);
	}
//--Settings delegates unbind end--//
}

void UAudioOptionsPage::LoadCurrentSettings()
{
	CurrentSettings = USaveGameUtils::LoadFromSlot<UProjectColorSaveSettings>(ESlotType::Settings);
	if (!CurrentSettings)
	{
		CurrentSettings = UProjectColorSaveSettings::CreateDefault();
	}
	ApplyCurrentSettingsToUIComponents();
}

void UAudioOptionsPage::ApplyCurrentSettingsToUIComponents()
{
	if (!IsValid(CurrentSettings)) { return; }

	GeneralVolumeSlider->SetValue(CurrentSettings->AudioGeneralVolume);
	MusicVolumeSlider->SetValue(CurrentSettings->AudioMusicVolume);
	EffectsVolumeSlider->SetValue(CurrentSettings->AudioEffectsVolume);

	SetMasterVolume(CurrentSettings->AudioGeneralVolume);
	SetMusicVolume(CurrentSettings->AudioMusicVolume);
	SetSFXVolume(CurrentSettings->AudioEffectsVolume);
}

void UAudioOptionsPage::SaveCurrentSettings()
{
	USaveGameUtils::SaveToSlot(ESlotType::Settings, CurrentSettings);
}

void UAudioOptionsPage::SetMasterVolume(float Volume)
{
	if (!IsValid(MasterSoundClass)) { return; }
	
	MasterSoundClass->Properties.Volume = Volume;

	if (MasterSoundMix)
	{
		UGameplayStatics::SetBaseSoundMix(this, MasterSoundMix);
	}
}

void UAudioOptionsPage::SetMusicVolume(float Volume)
{
	if (!IsValid(MusicSoundClass)) { return; }
	
	MusicSoundClass->Properties.Volume = Volume;

	if (MasterSoundMix)
	{
		UGameplayStatics::SetBaseSoundMix(this, MasterSoundMix);
	}
}

void UAudioOptionsPage::SetSFXVolume(float Volume)
{
	if (!IsValid(SFXSoundClass)) { return; }

	SFXSoundClass->Properties.Volume = Volume;

	if (MasterSoundMix)
	{
		UGameplayStatics::SetBaseSoundMix(this, MasterSoundMix);
	}
}

void UAudioOptionsPage::OnGraphicsButtonClicked()
{
	if (IsValid(HUD))
	{
		if (AMainMenuHUD* MenuHUD = Cast<AMainMenuHUD>(HUD))
		{
			if (MenuHUD)
			{
				MenuHUD->CreateGraphicOptionsPage();
			}
		}
		else if (AGameHUD* GameHUD = Cast<AGameHUD>(HUD))
		{
			if (GameHUD)
			{
				GameHUD->CreateGraphicOptionsPage();
			}
		}
		
	}
}

void UAudioOptionsPage::OnBackButtonClicked()
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

void UAudioOptionsPage::OnGeneralVolumeSliderChanged(const float Value)
{
	SetMasterVolume(Value);
}

void UAudioOptionsPage::OnMusicVolumeSliderChanged(const float Value)
{
	SetMusicVolume(Value);
}

void UAudioOptionsPage::OnEffectsVolumeSliderChanged(const float Value)
{
	SetSFXVolume(Value);
}

void UAudioOptionsPage::OnGeneralVolumeSliderReleased()
{
	CurrentSettings->AudioGeneralVolume = GeneralVolumeSlider->GetValue();
	SaveCurrentSettings();
}

void UAudioOptionsPage::OnMusicVolumeSliderReleased()
{
	CurrentSettings->AudioMusicVolume = MusicVolumeSlider->GetValue();
	SaveCurrentSettings();
}

void UAudioOptionsPage::OnEffectsVolumeSliderReleased()
{
	CurrentSettings->AudioEffectsVolume = EffectsVolumeSlider->GetValue();
	SaveCurrentSettings();
}
