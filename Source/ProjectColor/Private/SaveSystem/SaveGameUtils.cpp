#include "SaveSystem/SaveGameUtils.h"

#include "Kismet/GameplayStatics.h"

const TMap<ESlotType, FString> USaveGameUtils::SlotTypeNameMap = {
	{ESlotType::Game, TEXT("StdGameSlot")},
	{ESlotType::Settings, TEXT("StdGameSettings")},
};

const FString& USaveGameUtils::GetSlotNameFromType(ESlotType SlotType)
{
	const FString* FoundName = SlotTypeNameMap.Find(SlotType);
	checkf(FoundName, TEXT("Slot name for ESlotType %d not found!"), static_cast<uint8>(SlotType));
	return *FoundName;
}

void USaveGameUtils::DeleteSaveSlot(ESlotType SlotType)
{
	const FString& SlotName = GetSlotNameFromType(SlotType);

	if (!UGameplayStatics::DoesSaveGameExist(SlotName, DefaultSlotUserIndex))
	{
		return;
	}

	const bool bDeleted = UGameplayStatics::DeleteGameInSlot(SlotName, DefaultSlotUserIndex);

	UE_LOG(LogTemp, Log, TEXT("Game save '%s' deleted: %hs"), *SlotName, bDeleted ? "true" : "false");
}

void USaveGameUtils::SaveToSlot(ESlotType SlotType, USaveGame* SaveGame)
{
	const FString& SlotName = GetSlotNameFromType(SlotType);

	UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, DefaultSlotUserIndex);
}
