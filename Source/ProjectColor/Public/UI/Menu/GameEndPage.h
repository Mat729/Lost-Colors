#pragma once

#include "CoreMinimal.h"
#include "GamePageBase.h"
#include "GameEndPage.generated.h"

class UTextBlock;

UCLASS()
class PROJECTCOLOR_API UGameEndPage : public UGamePageBase
{
	GENERATED_BODY()
protected:
//--UUSerWidget overrides start--//
	virtual void NativeConstruct() override;
//--UUSerWidget overrides end--//
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> EndGameText;
};
