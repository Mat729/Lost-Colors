#pragma once

#include "GamePageBase.h"
#include "IntroPage.generated.h"

UCLASS()
class PROJECTCOLOR_API UIntroPage : public UGamePageBase
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;


private:
	UFUNCTION(BlueprintCallable, Category = IntroPage)
	void WidgetFadeOut();
};
