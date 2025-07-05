#pragma once

#include "GameFramework/Actor.h"
#include "SaveSystem/GameSavable.h"
#include "FragmentedObject.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UFragmentComponent;
class UCameraAlignmentComponent;
class USplineComponent;
class UNiagaraComponent;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleResolved);

USTRUCT(BlueprintType)
struct FFragmentedObjectState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bIsResolved;
};

UCLASS(ClassGroup="Project Color")
class PROJECTCOLOR_API AFragmentedObject : public AActor, public IGameSavable
{
	GENERATED_BODY()

public:
	AFragmentedObject();

	UPROPERTY(EditAnywhere, Category="Project Color|Fragmented Object")
	FString Identifier;

	FPuzzleResolved OnPuzzleResolved;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual FGameStateBlock FetchState() const override;

	virtual void ApplyState(const FGameStateBlock& Block) override;

private:
//-- Components Setup start--//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color|Fragmented Object", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> FragmentsContainer = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color|Fragmented Object", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraAlignmentComponent> CameraAlignmentComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color|Fragmented Object", meta = (AllowPrivateAccess = true))
	TObjectPtr<USplineComponent> MovementPath = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color|Fragmented Object", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> ResolutionVFX = nullptr;
//-- Components Setup end--/
	
	UPROPERTY(VisibleAnywhere, Category = "Project Color|Fragmented Object")
	TArray<UFragmentComponent*> Fragments = {}; 
	
	int32 FragmentsNumber = 0;

	int32 FragmentsReadyNumber = 0;

	bool bResolutionStarted = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Project Color|Fragmented Object", meta = (AllowPrivateAccess = true))
	float TimeToRebuildObject = 2.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Project Color|Fragmented Object", meta = (AllowPrivateAccess = true))
	float TimeToCompleteSplineMovement = 2.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Project Color|Fragmented Object", meta = (AllowPrivateAccess = true))
	float TimeToRecolorRoom = 5.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Project Color|Fragmented Object", meta = (AllowPrivateAccess = true))
	USoundBase* PuzzleResolvedSound = nullptr;

	void StopFloatingFragments();
	
	void UpdateFragmentedObjectVisuals(const bool bIsResolved);

public:
	//funzione chiamata quando un frammento aggiorna il suo stato
	UFUNCTION(BlueprintCallable, Category = "Project Color")
	void OnFragmentUpdate(const bool bFragmentReady, const bool bWasReady);
	
protected:	
	// funzione da chiamare quando si scatena l’evento della camera allineata
	UFUNCTION(BlueprintCallable, Category = "Project Color")
	void OnCorrectView();

	//funzione chiamata per iniziare la risoluzione dell'oggetto
	UFUNCTION(BlueprintImplementableEvent, Category = "Project Color")
	void ResolutionStart();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Project Color")
	void ColorRoomStart();
	
	//funzione di ricostruzione dell'oggetto
	UFUNCTION(BlueprintCallable, Category = "Project Color")
	void MoveFragmentsToOrigin(const float InAlpha);
	
	//funzione di movimento finale
	UFUNCTION(BlueprintCallable, Category = "Project Color")
	void MoveObjectToEndLocation(const float InAlpha);

	UFUNCTION(BlueprintCallable, Category = "Project Color")
	void PlayPuzzleResolvedSound() const;

	//funzione di completamento puzzle
	UFUNCTION(BlueprintCallable, Category = "Project Color")
	void PuzzleResolved(bool bShouldSaveGame = true);
};
