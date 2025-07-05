#include "UI/Menu/MainMenuPage.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SaveGameUtils.h"
#include "UI/Menu/GameButtonBaseWidget.h"
#include "UI/Menu/MainMenuHUD.h"

void UMainMenuPage::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = GetOwningPlayer();
	if (!IsValid(PlayerController)) {return;}
	MenuHUD = Cast<AMainMenuHUD>(PlayerController->GetHUD());

	SetButtonsBindings();
}

void UMainMenuPage::NativeDestruct()
{
	ResetButtonsBindings();

	Super::NativeDestruct();
}

void UMainMenuPage::SetButtonsBindings()
{
	//--Buttons delegates setup start--//
	if (IsValid(PlayButton))
	{
		PlayButton->ButtonClickedDelegate.AddDynamic(this, &UMainMenuPage::OnPlayButtonClicked);
	}

	if (IsValid(NewGameButton))
	{
		NewGameButton->ButtonClickedDelegate.AddDynamic(this, &UMainMenuPage::OnNewGameButtonClicked);
	}

	if (IsValid(OptionsButton))
	{
		OptionsButton->ButtonClickedDelegate.AddDynamic(this, &UMainMenuPage::OnOptionsButtonClicked);
	}

	if (IsValid(CreditsButton))
	{
		CreditsButton->ButtonClickedDelegate.AddDynamic(this, &UMainMenuPage::OnCreditsButtonClicked);
	}

	if (IsValid(QuitButton))
	{
		QuitButton->ButtonClickedDelegate.AddDynamic(this, &UMainMenuPage::OnQuitButtonClicked);
	}
	//--Buttons delegates setup end--//
}

void UMainMenuPage::ResetButtonsBindings()
{
	//--Buttons delegates unbind start--//
	if (IsValid(PlayButton))
	{
		PlayButton->ButtonClickedDelegate.RemoveDynamic(this, &UMainMenuPage::OnPlayButtonClicked);
	}

	if (IsValid(NewGameButton))
	{
		NewGameButton->ButtonClickedDelegate.RemoveDynamic(this, &UMainMenuPage::OnNewGameButtonClicked);
	}

	if (IsValid(OptionsButton))
	{
		OptionsButton->ButtonClickedDelegate.RemoveDynamic(this, &UMainMenuPage::OnOptionsButtonClicked);
	}

	if (IsValid(CreditsButton))
	{
		CreditsButton->ButtonClickedDelegate.RemoveDynamic(this, &UMainMenuPage::OnCreditsButtonClicked);
	}

	if (IsValid(QuitButton))
	{
		QuitButton->ButtonClickedDelegate.RemoveDynamic(this, &UMainMenuPage::OnQuitButtonClicked);
	}
	//--Buttons delegates unbind end--//
}

void UMainMenuPage::OnPlayButtonClicked()
{
	if (!IsValid(PlayerController)) {return;}
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level1"), false);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void UMainMenuPage::OnNewGameButtonClicked()
{
	if (!IsValid(PlayerController)) {return;}
	USaveGameUtils::DeleteSaveSlot(ESlotType::Game);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level1"), false);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void UMainMenuPage::OnOptionsButtonClicked()
{
	if (!IsValid(MenuHUD)) {return;}
	MenuHUD->CreateOptionsPage();
}

void UMainMenuPage::OnCreditsButtonClicked()
{
	if (!IsValid(MenuHUD)) {return;}
	MenuHUD->CreateCreditsPage();
}

void UMainMenuPage::OnQuitButtonClicked()
{
	if (!IsValid(MenuHUD)) {return;}
	MenuHUD->CreateQuitConfirmPage();
}
