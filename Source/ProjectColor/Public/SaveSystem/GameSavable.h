#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Interface.h"
#include "GameSavable.generated.h"

USTRUCT(BlueprintType)
struct FGameStateBlock
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString ID;

	UPROPERTY(BlueprintReadWrite)
	FInstancedStruct Data;
};

UINTERFACE(BlueprintType)
class UGameSavable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTCOLOR_API IGameSavable
{
	GENERATED_BODY()

public:
	virtual FGameStateBlock FetchState() const = 0;
	virtual void ApplyState(const FGameStateBlock& Block) = 0;
};
