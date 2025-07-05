#pragma once

#include "GamePageBase.h"
#include "MainMenuPage.generated.h"

class AMainMenuHUD;
class UGameButtonBaseWidget;

UCLASS()
class PROJECTCOLOR_API UMainMenuPage : public UGamePageBase
{
	GENERATED_BODY()

protected:
//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//--UUSerWidget overrides end--//
	
//--Buttons widget setup start--//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> PlayButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> NewGameButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> CreditsButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> QuitButton;
//--Buttons widget setup end--//

private:
	UPROPERTY()
	AMainMenuHUD* MenuHUD = nullptr;

	UPROPERTY()
	APlayerController* PlayerController = nullptr;
	
//--Buttons delegate binding setup start--//
	void SetButtonsBindings();
	void ResetButtonsBindings();
//--Buttons delegate binding setup end--//
	
//--Callbacks Setup start--//
	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnNewGameButtonClicked();
	
	UFUNCTION()
	void OnOptionsButtonClicked();
	
	UFUNCTION()
	void OnCreditsButtonClicked();
	
	UFUNCTION()
	void OnQuitButtonClicked();
//--Callbacks Setup start--//
};
