#pragma once

#include "Blueprint/UserWidget.h"
#include "GameButtonBaseWidget.generated.h"

class UTextBlock;
class UButton;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedDelegate);

UCLASS()
class PROJECTCOLOR_API UGameButtonBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
//--UUserWidget overrides--//
protected:
	virtual void NativePreConstruct() override;
//--UUserWidget overrides--//
	
protected:
//--Bindings--//
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button = nullptr;
		
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock = nullptr;
//--Bindings--//
public:
	void SetText(const FText& InText) const;

//--Delegate Setup--//
	FOnButtonClickedDelegate ButtonClickedDelegate;
//--Delegate Setup--//
	
protected:
//--Button Config--//
	UPROPERTY(EditAnywhere, Category= "ProjectColor | ButtonConfig")
	FText ButtonText;
//--Button Config--//

private:
	UFUNCTION()
	void OnButtonClicked();
};
