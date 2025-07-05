#include "UI/Menu/MainMenuHUD.h"


#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Menu/MainMenuPage.h"
#include "UI/Menu/QuitConfirmPage.h"
#include "UI/Menu/CreditsPage.h"
#include "UI/Menu/AudioOptionsPage.h"
#include "UI/Menu/GraphicOptionsPage.h"
#include "UI/Menu/OptionsPage.h"
#include "UI/Menu/IntroPage.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	CreateIntroPage();
	
	PlayerController = GetOwningPlayerController();
}

UGamePageBase* AMainMenuHUD::CreateWidgetAndAddToViewport(TSubclassOf<UGamePageBase> InWidgetClass, int32 InZOrder)
{
	ensureMsgf(InWidgetClass, TEXT("Invalid Widget class in HUD"));
	if (!InWidgetClass) { return nullptr; }
	UGamePageBase* GenericWidget = Cast<UGamePageBase>(UWidgetBlueprintLibrary::Create(this, InWidgetClass, GetOwningPlayerController()));

	ensureMsgf(GenericWidget, TEXT("HUD Widget creation failed"));
	if (!GenericWidget) { return nullptr; }
	GenericWidget->AddToViewport(InZOrder);
	return GenericWidget;
}

void AMainMenuHUD::AddPage(TSubclassOf<UGamePageBase> InPageClass, FName InPageName)
{
	UGamePageBase* addedPage = Cast<UGamePageBase>(CreateWidgetAndAddToViewport(InPageClass, 2));
	GamePagesMap.Emplace(InPageName, addedPage);
}

void AMainMenuHUD::RemovePage(FName InPageName)
{
	TObjectPtr<UGamePageBase>* pageBasePtrToPtr = GamePagesMap.Find(InPageName);
	if (pageBasePtrToPtr)
	{
		TObjectPtr<UGamePageBase> pageBasePtr = *pageBasePtrToPtr;
		pageBasePtr->RemoveFromParent();
	}

	GamePagesMap.Remove(InPageName);
}


void AMainMenuHUD::CreateIntroPage()
{
	AddPage(IntroPageClass, TEXT("IntroPage"));
	if (!IsValid(PlayerController)) { return; }
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetPause(false);
}

void AMainMenuHUD::CreateMainMenuPage()
{
	RemovePage("GraphicOptions");
	RemovePage("AudioOptions");
	RemovePage("Options");
	RemovePage("Credits");
	RemovePage("IntroPage");
	if (!IsValid(PlayerController)) { return; }
	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->SetShowMouseCursor(true);
	AddPage(MainMenuPageClass,"MainMenu" );
}

void AMainMenuHUD::CreateQuitConfirmPage()
{
	RemovePage("MainMenu");
	AddPage(QuitConfirmPageClass,"QuitConfirm");
}

void AMainMenuHUD::CreateCreditsPage()
{
	RemovePage("MainMenu");
	AddPage(CreditsPageClass,"Credits");
}

void AMainMenuHUD::CreateOptionsPage()
{
	RemovePage("MainMenu");
	AddPage(OptionsPageClass,"Options");
}

void AMainMenuHUD::CreateAudioOptionsPage()
{
	RemovePage("Options");
	RemovePage("GraphicOptions");
	AddPage(AudioOptionsPageClass,"AudioOptions");
}

void AMainMenuHUD::CreateGraphicOptionsPage()
{
	RemovePage("Options");
	RemovePage("AudioOptions");
	AddPage(GraphicOptionsPageClass,"GraphicOptions");
}


