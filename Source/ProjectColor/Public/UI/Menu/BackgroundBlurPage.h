#pragma once

#include "GamePageBase.h"
#include "BackgroundBlurPage.generated.h"

class UBackgroundBlur;

UCLASS()
class PROJECTCOLOR_API UBackgroundBlurPage : public UGamePageBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBackgroundBlur>	Blur;
};
