#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectColorGameMode.generated.h"

class AFragmentedObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelCompleted);

UCLASS(minimalapi)
class AProjectColorGameMode : public AGameModeBase
{
	GENERATED_BODY()

	/** Holds the resolution status for all FragmentedObjects. */
	TMap<FString, bool> PuzzleState;

	/** Holds the integer key for editor debugging */
	const int GameStateDebugKey = 101;

	/** Holds the timer handle used for auto saving game */
	FTimerHandle AutoSaveTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Save", meta = (AllowPrivateAccess = "true"))
	float AutoSaveFrequencySeconds = 5.f;
	
public:
	AProjectColorGameMode();

	UFUNCTION(BlueprintCallable, Category = "Project Color|Game Mode")
	void FragmentsPuzzleResolved(AFragmentedObject* FragmentedObject, bool bShouldSaveGame = true);

	UFUNCTION(BlueprintCallable, Category = "Project Color|Game Mode")
	void SaveGame() const;

	FLevelCompleted OnLevelCompleted;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void InitPuzzleState();
	bool IsLevelCompleted() const;
	void UpdateLevelProgress();
	void LoadGame() const;
	void StartAutoSaveScheduler();
	void StopAutoSaveScheduler();
	void ApplyGraphicsSettings() const;
};
