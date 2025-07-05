#pragma once

#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UGraphicOptionsPage;
class UAudioOptionsPage;
class UIntroPage;
class UGamePageBase;
class UCreditsPage;
class UOptionsPage;
class UQuitConfirmPage;
class UMainMenuPage;

UCLASS()
class PROJECTCOLOR_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

//--Create Widgets Setup begin--//
	UPROPERTY(EditAnywhere)
	TSubclassOf<UIntroPage> IntroPageClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuitConfirmPage> QuitConfirmPageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOptionsPage> OptionsPageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCreditsPage> CreditsPageClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuPage> MainMenuPageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAudioOptionsPage> AudioOptionsPageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGraphicOptionsPage> GraphicOptionsPageClass;
	
private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;
	
	UGamePageBase* CreateWidgetAndAddToViewport(TSubclassOf<UGamePageBase> InWidgetClass, int32 InZOrder);
	
public:
	void AddPage(TSubclassOf<UGamePageBase> InPageClass, FName InPageName);

	void RemovePage(FName InPageName);

private:
	TMap<FName, TObjectPtr<UGamePageBase>> GamePagesMap = {};
//--Create Widgets Setup end--//

public:
//--Callbacks setup start--//
	void CreateIntroPage();

	UFUNCTION(BlueprintCallable, Category = "ProjectColor | HUD")
	void CreateMainMenuPage();

	void CreateQuitConfirmPage();

	void CreateCreditsPage();

	void CreateOptionsPage();

	void CreateAudioOptionsPage();
	
	void CreateGraphicOptionsPage();
//--Callbacks setup end--//
};
