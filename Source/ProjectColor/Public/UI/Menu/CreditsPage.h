#pragma once

#include "GamePageBase.h"
#include "CreditsPage.generated.h"

class UGameButtonBaseWidget;
class AMainMenuHUD;

UCLASS()
class PROJECTCOLOR_API UCreditsPage : public UGamePageBase
{
	GENERATED_BODY()

protected:
//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//--UUSerWidget overrides end--//
	
//--Buttons widget setup start--//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> BackButton;
//--Buttons widget setup end--//

private:
	UPROPERTY()
	AMainMenuHUD* MenuHUD = nullptr;

//--Buttons delegate binding setup start--//
	void SetButtonsBindings();
	void ResetButtonsBindings();
//--Buttons delegate binding setup end--//
	
//--Callbacks Setup start--//
	UFUNCTION()
	void OnBackButtonClicked();
//--Callbacks Setup end--//
};
