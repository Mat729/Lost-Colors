#pragma once

#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

class UImage;

UCLASS()
class PROJECTCOLOR_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
//--User Widget Overrides--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
//--User Widget Overrides--//

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Crosshair")
	TObjectPtr<UImage> Crosshair = nullptr;
};
