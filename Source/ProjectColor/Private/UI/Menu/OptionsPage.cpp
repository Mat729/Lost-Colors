#include "UI/Menu/OptionsPage.h"

#include "UI/GameHUD.h"
#include "UI/Menu/GameButtonBaseWidget.h"
#include "UI/Menu/MainMenuHUD.h"

void UOptionsPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	HUD = GetOwningPlayer()->GetHUD();

	SetButtonsBindings();
}

void UOptionsPage::NativeDestruct()
{
	ResetButtonsBindings();

	Super::NativeDestruct();
}

void UOptionsPage::SetButtonsBindings()
{
//--Buttons delegates setup start--//
	if (IsValid(AudioButton))
	{
		AudioButton->ButtonClickedDelegate.AddDynamic(this, &UOptionsPage::OnAudioButtonClicked);
	}

	if (IsValid(GraphicsButton))
	{
		GraphicsButton->ButtonClickedDelegate.AddDynamic(this, &UOptionsPage::OnGraphicsButtonClicked);
	}

	if (IsValid(BackButton))
	{
		BackButton->ButtonClickedDelegate.AddDynamic(this, &UOptionsPage::OnBackButtonClicked);
	}
//--Buttons delegates setup end--//
}

void UOptionsPage::ResetButtonsBindings()
{
//--Buttons delegates unbind start--//
	if (IsValid(AudioButton))
	{
		AudioButton->ButtonClickedDelegate.RemoveDynamic(this, &UOptionsPage::OnAudioButtonClicked);
	}

	if (IsValid(GraphicsButton))
	{
		GraphicsButton->ButtonClickedDelegate.RemoveDynamic(this, &UOptionsPage::OnGraphicsButtonClicked);
	}

	if (IsValid(BackButton))
	{
		BackButton->ButtonClickedDelegate.RemoveDynamic(this, &UOptionsPage::OnBackButtonClicked);
	}
//--Buttons delegates unbind end--//
}

void UOptionsPage::OnAudioButtonClicked()
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

void UOptionsPage::OnGraphicsButtonClicked()
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

void UOptionsPage::OnBackButtonClicked()
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
