#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UBackgroundBlurPage;
class UGraphicOptionsPage;
class UAudioOptionsPage;
class UOptionsPage;
class UGamePageBase;
class UGameHUDPage;
class UPausePage;

UCLASS()
class PROJECTCOLOR_API AGameHUD : public AHUD
{
	GENERATED_BODY()

	
protected:
	virtual void BeginPlay() override;

//--Create Widgets Setup begin--//
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameHUDPage> GamePageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPausePage> PausePageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOptionsPage> OptionsPageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAudioOptionsPage> AudioOptionsPageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGraphicOptionsPage> GraphicOptionsPageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBackgroundBlurPage> BackgroundBlurPageClass;
//--Create Widgets Setup end--//
	
private:
//--Generic widget creation function--//
	UGamePageBase* CreateWidgetAndAddToViewport(TSubclassOf<UGamePageBase> InWidgetClass, int32 InZOrder);

	TMap<FName, TObjectPtr<UGamePageBase>> GamePagesMap = {};
//--Generic widget creation function--//

	UPROPERTY()
	APlayerController* PlayerController = nullptr;

public:
//--Page management functions--//
	void OnPause();
	
	void OnResume();

	UFUNCTION(BlueprintCallable)
	void AddPage(TSubclassOf<UGamePageBase> InPageClass, FName InPageName, int32 InZOrder);

	UFUNCTION(BlueprintCallable)
	void RemovePage(FName InPageName);

	void CreateOptionsPage();
	
	void CreateAudioOptionsPage();
	
	void CreateGraphicOptionsPage();
//--Page management functions--//
};
