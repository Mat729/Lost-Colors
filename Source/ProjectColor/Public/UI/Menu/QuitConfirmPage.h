#pragma once

#include "GamePageBase.h"
#include "QuitConfirmPage.generated.h"

class UGameButtonBaseWidget;

UCLASS()
class PROJECTCOLOR_API UQuitConfirmPage : public UGamePageBase
{
	GENERATED_BODY()

protected:
//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//--UUSerWidget overrides end--//

//--Buttons widget setup start--//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> QuitConfirmButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> StayButton;
//--Buttons widget setup end--//

private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;
	
//--Buttons delegate binding setup start--//
	void SetButtonsBindings();
	void ResetButtonsBindings();
//--Buttons delegate binding setup end--//
	
//--Callbacks Setup start--//
	UFUNCTION()
	void OnQuitConfirmButtonClicked();

	UFUNCTION()
	void OnStayButtonClicked();
//--Callbacks Setup end--//
};
