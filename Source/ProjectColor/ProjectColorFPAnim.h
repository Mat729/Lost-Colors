#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProjectColorFPAnim.generated.h"

UCLASS()
class PROJECTCOLOR_API UProjectColorFPAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool HasRifle = false;
};
