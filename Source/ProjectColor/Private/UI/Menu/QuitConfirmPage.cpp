#include "UI/Menu/QuitConfirmPage.h"

#include "UI/Menu/GameButtonBaseWidget.h"
#include "UI/Menu/MainMenuHUD.h"

void UQuitConfirmPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetButtonsBindings();
	
	PlayerController = GetOwningPlayer();
}

void UQuitConfirmPage::NativeDestruct()
{
	ResetButtonsBindings();

	Super::NativeDestruct();
}

void UQuitConfirmPage::SetButtonsBindings()
{
//--Buttons delegates setup start--//
	if (IsValid(QuitConfirmButton))
	{
		QuitConfirmButton->ButtonClickedDelegate.AddDynamic(this, &UQuitConfirmPage::OnQuitConfirmButtonClicked);
	}
	if (IsValid(StayButton))
	{
		StayButton->ButtonClickedDelegate.AddDynamic(this, &UQuitConfirmPage::OnStayButtonClicked);
	}
//--Buttons delegates setup end--//
}

void UQuitConfirmPage::ResetButtonsBindings()
{
//--Buttons delegates unbind start--//
	if (IsValid(QuitConfirmButton))
	{
		QuitConfirmButton->ButtonClickedDelegate.RemoveDynamic(this, &UQuitConfirmPage::OnQuitConfirmButtonClicked);
	}
	if (IsValid(StayButton))
	{
		StayButton->ButtonClickedDelegate.RemoveDynamic(this, &UQuitConfirmPage::OnStayButtonClicked);
	}
//--Buttons delegates unbind end--//
}

void UQuitConfirmPage::OnQuitConfirmButtonClicked()
{
	if (!IsValid(PlayerController)) {return;}
	PlayerController->ConsoleCommand("Quit");
}

void UQuitConfirmPage::OnStayButtonClicked()
{
	if (!IsValid(PlayerController)) {return;}
	AMainMenuHUD* MenuHUD = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	if (!MenuHUD) {return;}
	MenuHUD->RemovePage("QuitConfirm");
	MenuHUD->CreateMainMenuPage();
}
