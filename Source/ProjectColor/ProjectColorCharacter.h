// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SaveSystem/GameSavable.h"
#include "ProjectColorCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USoundClass;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT(BlueprintType)
struct FCharacterState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector WorldLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	FRotator WorldRotation = FRotator::ZeroRotator;
};

UCLASS(config=Game)
class AProjectColorCharacter : public ACharacter, public IGameSavable
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FocusAction;

	const FString& SaveBlockId = FString("Character");
	
public:
	AProjectColorCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project Color | Focus Mode")
	float FocusFieldOfView = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project Color | Focus Mode")
	float CameraTransitionSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project Color | Focus Mode")
	float FocusLowPassFilterFrequency = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project Color | Focus Mode")
	float SoundTransitionSpeed = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project Color | Focus Mode")
	TObjectPtr<USoundClass> MusicSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Project Color | Focus Mode")
	float TimeToFocus = 2.0f;

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for enabling focus mode */
	void Focus(const FInputActionValue& Value);

// 	IGameSavable
	virtual void ApplyState(const FGameStateBlock& Block) override;
	virtual FGameStateBlock FetchState() const override;
//  end IGameSavable
	
private:
	/** Holds the timer handle used for delayed focus flag activation */
	FTimerHandle FocusTimerHandle;

	/** Holds the flag activated/disactivated when focus input has been triggered/released */
	bool bTryingToFocus = false;

	/** Holds the flag representing the focused state */
	bool bIsFocused = false;

	/** Holds the default player character camera FOV */
	float DefaultFieldOfView = 0.f;

	float DefaultLowPassFilterFrequency = 0.f;

	void DelayedActivateFocus();
	void ActivateFocus();
	void DeactivateFocus();

	/**
	 * Smoothly interpolates the camera's Field of View (FOV) based on the character's focus state.
	 *
	 * If the character is trying to focus (`bTryingToFocus` is true), the camera FOV is smoothly transitioned 
	 * towards a narrower `FocusFieldOfView` to simulate a zoom-in effect. Otherwise, it interpolates back to 
	 * the default `DefaultFieldOfView`.
	 *
	 * The Uproperties CameraTransitionSpeed and FocusFieldOfView customize the zoom-in/zoom-out effect.
	 *
	 * @param DeltaSeconds The time elapsed since the last tick, used to make the interpolation frame-rate independent.
	 */
	void HandleFocusTick(const float DeltaSeconds);

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	bool IsFocusModeRunning() const;

	bool IsTryingToFocus() const { return bTryingToFocus; }
};

