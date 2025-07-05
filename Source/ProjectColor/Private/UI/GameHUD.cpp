#include "UI/GameHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/GameHUDPage.h"
#include "UI/Menu/GamePageBase.h"
#include "UI/Menu/PausePage.h"
#include "UI/Menu/OptionsPage.h"
#include "UI/Menu/AudioOptionsPage.h"
#include "UI/Menu/GraphicOptionsPage.h"
#include "UI/Menu/BackgroundBlurPage.h"


void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetOwningPlayerController();
	
	AddPage(GamePageClass, "GameHUD",2);

}

UGamePageBase* AGameHUD::CreateWidgetAndAddToViewport(TSubclassOf<UGamePageBase> InWidgetClass, int32 InZOrder)
{
	ensureMsgf(InWidgetClass, TEXT("Invalid Widget class in HUD"));
	if (!InWidgetClass) { return nullptr; }

	UGamePageBase* GenericWidget = Cast<UGamePageBase>(UWidgetBlueprintLibrary::Create(this, InWidgetClass, GetOwningPlayerController()));
	ensureMsgf(GenericWidget, TEXT("HUD Widget creation failed"));
	if (!GenericWidget) { return nullptr; }
	
	GenericWidget->AddToViewport(InZOrder);
	return GenericWidget;
}


void AGameHUD::OnPause()
{
	if (!IsValid(PlayerController)) { return; }
	RemovePage("GameHUD");
	RemovePage("OptionsPage");
	RemovePage("AudioOptionsPage");
	RemovePage("GraphicOptionsPage");
	RemovePage("BlurPage");
	AddPage(BackgroundBlurPageClass,"BlurPage",0);
	AddPage(PausePageClass, "PausePage",2);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(FInputModeUIOnly());
}

void AGameHUD::OnResume()
{
	if (!IsValid(PlayerController)) { return; }
	RemovePage("PausePage");
	RemovePage("BlurPage");
	AddPage(GamePageClass, "GameHUD", 2);
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void AGameHUD::AddPage(TSubclassOf<UGamePageBase> InPageClass, FName InPageName, int32 InZOrder)
{
	UGamePageBase* addedPage = Cast<UGamePageBase>(CreateWidgetAndAddToViewport(InPageClass, InZOrder));
	GamePagesMap.Emplace(InPageName, addedPage);
}

void AGameHUD::RemovePage(FName InPageName)
{
	TObjectPtr<UGamePageBase>* pageBasePtrToPtr = GamePagesMap.Find(InPageName);
	if (pageBasePtrToPtr)
	{
		TObjectPtr<UGamePageBase> pageBasePtr = *pageBasePtrToPtr;
		pageBasePtr->RemoveFromParent();
	}

	GamePagesMap.Remove(InPageName);
}

void AGameHUD::CreateOptionsPage()
{
	RemovePage("PausePage");
	AddPage(OptionsPageClass,"OptionsPage",2);
}

void AGameHUD::CreateAudioOptionsPage()
{
	RemovePage("OptionsPage");
	RemovePage("GraphicOptionsPage");
	AddPage(AudioOptionsPageClass,"AudioOptionsPage",2);
}

void AGameHUD::CreateGraphicOptionsPage()
{
	RemovePage("OptionsPage");
	RemovePage("AudioOptionsPage");
	AddPage(GraphicOptionsPageClass,"GraphicOptionsPage",2);
}
