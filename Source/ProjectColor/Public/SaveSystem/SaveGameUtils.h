#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameUtils.generated.h"

class USaveGame;

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	/** The save slot reserved for runtime game data. */
	Game UMETA(DisplayName = "Game"),

	/** The save slot reserved for user game settings. */
	Settings UMETA(DisplayName = "Settings")
};

UCLASS()
class PROJECTCOLOR_API USaveGameUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Const slots string identifiers */
	static const FString& GameSlotName;
	static const FString& SettingsSlotName;

	/** Const save slot user index */
	static constexpr uint8 DefaultSlotUserIndex = 0;

protected:
	static const FString& GetSlotNameFromType(ESlotType SlotType);

public:
	UFUNCTION(BlueprintCallable, Category = "Project Color | Save Game")
	static void DeleteSaveSlot(ESlotType SlotType);

	UFUNCTION(BlueprintCallable, Category = "Project Color | Save Game")
	static void SaveToSlot(ESlotType SlotType, USaveGame* SaveGame);

	/**
	 * @return nullable USaveGame* data casted to template type
	 */
	template <typename T>
	UFUNCTION(BlueprintCallable, Category = "Project Color | Save Game")
	static T* LoadFromSlot(ESlotType SlotType)
	{
		const FString& SlotName = GetSlotNameFromType(SlotType);

		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, DefaultSlotUserIndex);
		if (!SaveGame) { return nullptr; }

		return Cast<T>(SaveGame);
	}

private:
	static const TMap<ESlotType, FString> SlotTypeNameMap;
};
