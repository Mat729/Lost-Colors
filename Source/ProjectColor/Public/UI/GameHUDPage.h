#pragma once

#include "Menu/GamePageBase.h"
#include "GameHUDPage.generated.h"

class AGameHUD;

UCLASS()
class PROJECTCOLOR_API UGameHUDPage : public UGamePageBase
{
	GENERATED_BODY()

protected:
	//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//--UUSerWidget overrides end--//

private:
	UPROPERTY()
	TWeakObjectPtr<AGameHUD> GameHUD = nullptr;

	UFUNCTION(BlueprintCallable)
	void PauseMenu() const;
};
