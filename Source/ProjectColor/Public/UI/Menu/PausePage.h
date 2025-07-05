#pragma once

#include "GamePageBase.h"
#include "PausePage.generated.h"

class UBackgroundBlur;
class AGameHUD;
class UGameButtonBaseWidget;

UCLASS()
class PROJECTCOLOR_API UPausePage : public UGamePageBase
{
	GENERATED_BODY()

protected:
//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//--UUSerWidget overrides end--//

protected:
//--Buttons widget setup start--//
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGameButtonBaseWidget>  MainMenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGameButtonBaseWidget>  ResumeButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UGameButtonBaseWidget>  OptionsButton;
//--Buttons widget setup end--//
	
private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY()
	AGameHUD* GameHUD = nullptr;
	
//--Buttons delegate binding setup start--//
	void SetButtonsBindings();
	void ResetButtonsBindings();
//--Buttons delegate binding setup end--//
private:
	UFUNCTION()
	void OnMainMenuButtonClicked();

	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnOptionsButtonClicked();
};
