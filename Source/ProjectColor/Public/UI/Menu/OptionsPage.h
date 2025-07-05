#pragma once

#include "GamePageBase.h"
#include "OptionsPage.generated.h"

class AHUD;
class UGameButtonBaseWidget;

UCLASS()
class PROJECTCOLOR_API UOptionsPage : public UGamePageBase
{
	GENERATED_BODY()
	
protected:
//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//--UUSerWidget overrides end--//
	
//--Buttons widget setup start--//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> AudioButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> GraphicsButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UGameButtonBaseWidget> BackButton;
//--Buttons widget setup end--//

private:
	UPROPERTY()
	AHUD* HUD = nullptr;
	
//--Buttons delegate binding setup start--//
	void SetButtonsBindings();
	void ResetButtonsBindings();
//--Buttons delegate binding setup end--//
	
//--Callbacks Setup start--//
	UFUNCTION()
	void OnAudioButtonClicked();

	UFUNCTION()
	void OnGraphicsButtonClicked();
	
	UFUNCTION()
	void OnBackButtonClicked();
//--Callbacks Setup end--//
};
