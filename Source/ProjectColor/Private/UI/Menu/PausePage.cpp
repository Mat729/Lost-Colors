#include "UI/Menu/PausePage.h"

#include "Kismet/GameplayStatics.h"
#include "UI/GameHUD.h"
#include "UI/Menu/GameButtonBaseWidget.h"

void UPausePage::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		GameHUD=Cast<AGameHUD>(PlayerController->GetHUD());
	}
	
	SetButtonsBindings();
}

void UPausePage::NativeDestruct()
{
	ResetButtonsBindings();

	Super::NativeDestruct();
}

void UPausePage::SetButtonsBindings()
{
//--Buttons delegates setup start--//
	if (MainMenuButton)
	{
		MainMenuButton->ButtonClickedDelegate.AddDynamic(this, &UPausePage::OnMainMenuButtonClicked);
	}
	if (ResumeButton)
	{
		ResumeButton->ButtonClickedDelegate.AddDynamic(this, &UPausePage::OnResumeButtonClicked);
	}
	if (OptionsButton)
	{
		OptionsButton->ButtonClickedDelegate.AddDynamic(this, &UPausePage::OnOptionsButtonClicked);
	}
//--Buttons delegates setup end--//
}

void UPausePage::ResetButtonsBindings()
{
//--Buttons delegates unbind start--//
	if (MainMenuButton)
	{
		MainMenuButton->ButtonClickedDelegate.RemoveDynamic(this, &UPausePage::OnMainMenuButtonClicked);
	}
	if (ResumeButton)
	{
		ResumeButton->ButtonClickedDelegate.RemoveDynamic(this, &UPausePage::OnResumeButtonClicked);
	}
	if (OptionsButton)
	{
		OptionsButton->ButtonClickedDelegate.RemoveDynamic(this, &UPausePage::OnOptionsButtonClicked);
	}
//--Buttons delegates unbind end--//
}

void UPausePage::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"), true);
	PlayerController->SetInputMode(FInputModeGameAndUI());
}

void UPausePage::OnResumeButtonClicked()
{
	if (!IsValid(GameHUD)) {return;}
	GameHUD->OnResume();

	if (!IsValid(PlayerController)) { return; }
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void UPausePage::OnOptionsButtonClicked()
{
	if (!IsValid(GameHUD)) {return;}
	GameHUD->CreateOptionsPage();
}
