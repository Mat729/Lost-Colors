#include "UI/GameHUDPage.h"

#include "UI/GameHUD.h"

void UGameHUDPage::NativeConstruct()
{
	Super::NativeConstruct();

	GameHUD = MakeWeakObjectPtr(Cast<AGameHUD>(GetOwningPlayer()->GetHUD()));
}

void UGameHUDPage::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGameHUDPage::PauseMenu() const
{
	if (!GameHUD.IsValid()){return;}
	GameHUD->OnPause();
}
