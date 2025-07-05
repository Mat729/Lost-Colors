// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectColorCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProjectColorCharacter

AProjectColorCharacter::AProjectColorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AProjectColorCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	HandleFocusTick(DeltaSeconds);
}

void AProjectColorCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFieldOfView = FirstPersonCameraComponent->FieldOfView;

	if (!IsValid(MusicSoundClass)) { return; }
	DefaultLowPassFilterFrequency = MusicSoundClass->Properties.LowPassFilterFrequency;
}

//////////////////////////////////////////////////////////////////////////// Input

void AProjectColorCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AProjectColorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectColorCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectColorCharacter::Look);

		// Focusing
		EnhancedInputComponent->BindAction(FocusAction, ETriggerEvent::Triggered, this, &AProjectColorCharacter::Focus);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

bool AProjectColorCharacter::IsFocusModeRunning() const
{
	return bIsFocused;
}


void AProjectColorCharacter::Move(const FInputActionValue& Value)
{
	// can't move while focusing
	if (bTryingToFocus)
	{
		return;
	}
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AProjectColorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjectColorCharacter::Focus(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	
	if (AxisValue > 0)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(FocusTimerHandle))
		{
			bTryingToFocus = true;
			GetWorld()->GetTimerManager().SetTimer(FocusTimerHandle, this, &AProjectColorCharacter::DelayedActivateFocus, TimeToFocus, false);
		}
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(FocusTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(FocusTimerHandle);
		}
		bTryingToFocus = false;
		DeactivateFocus();
	}
}

void AProjectColorCharacter::ApplyState(const FGameStateBlock& Block)
{
	const FCharacterState& CharacterState = Block.Data.Get<FCharacterState>();
	if (CharacterState.WorldLocation.Equals(FVector::ZeroVector)) { return; }

	// teleport character to the saved location
	SetActorLocation(CharacterState.WorldLocation, false, nullptr, ETeleportType::TeleportPhysics);

	// adjust rotation as well
	SetActorRotation(CharacterState.WorldRotation, ETeleportType::TeleportPhysics);
	if (Controller)
	{
		Controller->SetControlRotation(CharacterState.WorldRotation);
	}
}

FGameStateBlock AProjectColorCharacter::FetchState() const
{
	const FVector Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();
	const FCharacterState State = {Location, Rotation};
	return {
		SaveBlockId,
		FInstancedStruct::Make(State)
	};
}

void AProjectColorCharacter::DelayedActivateFocus()
{
	if (bTryingToFocus)
	{
		ActivateFocus();
	}
}

void AProjectColorCharacter::ActivateFocus()
{
	bIsFocused = true;
}

void AProjectColorCharacter::DeactivateFocus()
{
	bIsFocused = false;
}

void AProjectColorCharacter::HandleFocusTick(const float DeltaSeconds)
{
	float TargetFieldOfView = DefaultFieldOfView;
	float TargetLowPassFilterFrequency = DefaultLowPassFilterFrequency;
	
	if (bTryingToFocus)
	{
		TargetFieldOfView = FocusFieldOfView;
		TargetLowPassFilterFrequency = FocusLowPassFilterFrequency;
	}
	
	// change camera fov if needed
	if (FirstPersonCameraComponent->FieldOfView != TargetFieldOfView)
	{
		const float NewFOV = FMath::FInterpTo(FirstPersonCameraComponent->FieldOfView, TargetFieldOfView, DeltaSeconds, CameraTransitionSpeed);
		FirstPersonCameraComponent->SetFieldOfView(NewFOV);
	}

	if (!IsValid(MusicSoundClass)) { return; }
	if (MusicSoundClass->Properties.LowPassFilterFrequency != TargetLowPassFilterFrequency)
	{
		const float NewLPFF = FMath::FInterpTo(MusicSoundClass->Properties.LowPassFilterFrequency, TargetLowPassFilterFrequency, DeltaSeconds, SoundTransitionSpeed);
		MusicSoundClass->Properties.LowPassFilterFrequency = NewLPFF;
	}
}
