// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectColorGameMode.h"

#include "EngineUtils.h"
#include "Graphics/GlobalPostProcessManager.h"
#include "Graphics/GraphicsUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Mechanics/FragmentsLogic/FragmentedObject.h"
#include "SaveSystem/GameSavable.h"
#include "SaveSystem/ProjectColorSaveGame.h"
#include "SaveSystem/ProjectColorSaveSettings.h"
#include "SaveSystem/SaveGameUtils.h"
#include "UObject/ConstructorHelpers.h"

AProjectColorGameMode::AProjectColorGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AProjectColorGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitPuzzleState();

	LoadGame();

	StartAutoSaveScheduler();

	// spawn the post process manager into the world
	GetWorld()->SpawnActor<AGlobalPostProcessManager>();

	ApplyGraphicsSettings();
}

void AProjectColorGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopAutoSaveScheduler();
	
	Super::EndPlay(EndPlayReason);
}

void AProjectColorGameMode::InitPuzzleState()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFragmentedObject::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AFragmentedObject* FragmentedObject = Cast<AFragmentedObject>(Actor);
		if (FragmentedObject)
		{
			PuzzleState.Emplace(FragmentedObject->Identifier, false);
		}
	}
}

bool AProjectColorGameMode::IsLevelCompleted() const
{
	for (const auto Element : PuzzleState)
	{
		if (!Element.Value)
		{
			return false;
		}
	}
	return true;
}

void AProjectColorGameMode::FragmentsPuzzleResolved(AFragmentedObject* FragmentedObject, bool bShouldSaveGame)
{
	if (!FragmentedObject) return;

	PuzzleState.Emplace(FragmentedObject->Identifier, true);

	if (bShouldSaveGame)
	{
		// save the game
		SaveGame();
	}

	UpdateLevelProgress();
}

void AProjectColorGameMode::UpdateLevelProgress()
{
	if (!IsLevelCompleted())
	{
#if WITH_EDITORONLY_DATA
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::White, "Game not completed yet");
		}
#endif
		return;
	}

#if WITH_EDITORONLY_DATA
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Green, "Game completed");
	}
#endif

	StopAutoSaveScheduler();
	
	OnLevelCompleted.Broadcast();
}

void AProjectColorGameMode::SaveGame() const
{
	if (IsLevelCompleted()) { return; }
		
	UProjectColorSaveGame* SaveObject = Cast<UProjectColorSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UProjectColorSaveGame::StaticClass()));

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (const IGameSavable* GameSavable = Cast<IGameSavable>(Actor))
		{
			FGameStateBlock Block = GameSavable->FetchState();
			SaveObject->SavedBlocks.Add(Block.ID, Block);
		}
	}

	USaveGameUtils::SaveToSlot(ESlotType::Game, SaveObject);

#if WITH_EDITORONLY_DATA
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(GameStateDebugKey, 10.f, FColor::White, "Game saved");
	}
#endif
}

void AProjectColorGameMode::LoadGame() const
{
	UProjectColorSaveGame* SaveObject = USaveGameUtils::LoadFromSlot<UProjectColorSaveGame>(ESlotType::Game);
	if (!SaveObject) { return; }

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (IGameSavable* GameSavable = Cast<IGameSavable>(Actor))
		{
			FString ID = GameSavable->FetchState().ID;
			if (const FGameStateBlock* Block = SaveObject->SavedBlocks.Find(ID))
			{
				GameSavable->ApplyState(*Block);
			}
		}
	}

#if WITH_EDITORONLY_DATA
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(GameStateDebugKey, 10.f, FColor::White, "Game state loaded");
	}
#endif
}

void AProjectColorGameMode::StartAutoSaveScheduler()
{
	// start auto save timer
	GetWorld()->GetTimerManager().SetTimer(AutoSaveTimerHandle, this, &AProjectColorGameMode::SaveGame, AutoSaveFrequencySeconds, true);
}

void AProjectColorGameMode::StopAutoSaveScheduler()
{
	// clear auto save timer
	GetWorld()->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
}

void AProjectColorGameMode::ApplyGraphicsSettings() const
{
	const UProjectColorSaveSettings* GameSettings = USaveGameUtils::LoadFromSlot<UProjectColorSaveSettings>(ESlotType::Settings);
	if (!GameSettings) { return; }

	UGraphicsUtils::SetGlobalBrightness(this, GameSettings->GraphicsBrightness);
}
