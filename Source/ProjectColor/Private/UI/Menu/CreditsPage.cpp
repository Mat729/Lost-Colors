#include "UI/Menu/CreditsPage.h"

#include "UI/Menu/GameButtonBaseWidget.h"
#include "UI/Menu/MainMenuHUD.h"

void UCreditsPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	MenuHUD = Cast<AMainMenuHUD>(GetOwningPlayer()->GetHUD());

	SetButtonsBindings();
}

void UCreditsPage::NativeDestruct()
{
	ResetButtonsBindings();

	Super::NativeDestruct();
}

void UCreditsPage::SetButtonsBindings()
{
//--Buttons delegates setup start--//
	if (IsValid(BackButton))
	{
		BackButton->ButtonClickedDelegate.AddDynamic(this, &UCreditsPage::OnBackButtonClicked);
	}
//--Buttons delegates setup end--//
}

void UCreditsPage::ResetButtonsBindings()
{
//--Buttons delegates unbind start--//
	if (IsValid(BackButton))
	{
		BackButton->ButtonClickedDelegate.RemoveDynamic(this, &UCreditsPage::OnBackButtonClicked);
	}
//--Buttons delegates unbind end--//
}

void UCreditsPage::OnBackButtonClicked()
{
	if (!IsValid(MenuHUD)) {return;}
	MenuHUD->CreateMainMenuPage();
}
