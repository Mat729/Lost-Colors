#pragma once

#include "CoreMinimal.h"
#include "GameSavable.h"
#include "GameFramework/SaveGame.h"
#include "ProjectColorSaveGame.generated.h"

UCLASS()
class PROJECTCOLOR_API UProjectColorSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Holds the savable components/actors states */
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FGameStateBlock> SavedBlocks;
};
