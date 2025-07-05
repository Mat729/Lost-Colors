#include "Mechanics/Shooting/ColorCasterComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Mechanics/Shooting/Projectiles/ColorProjectile.h"
#include "ProjectColor/ProjectColorCharacter.h"

// Sets default values for this component's properties
UColorCasterComponent::UColorCasterComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UColorCasterComponent::Fire()
{
	FireColorProjectile(ActiveProjectileClass);
}

void UColorCasterComponent::FireEraser()
{
	FireColorProjectile(EraserProjectileClass);
}

void UColorCasterComponent::LoadProjectile(const FInputActionValue& ActionValue)
{
	const float AxisValue = ActionValue.Get<float>();

	const int32 Index = ProjectileClasses.IndexOfByPredicate([this](const TSubclassOf<class AColorProjectile>& Item)
	{
		return Item->GetName() == ActiveProjectileClass->GetName();
	});

	if (Index < 0)
	{
		return;
	}

	int32 NewIndex = Index + AxisValue;
	if (NewIndex > ProjectileClasses.Num() - 1)
	{
		NewIndex = 0;
	}
	else if (NewIndex < 0)
	{
		NewIndex = ProjectileClasses.Num() - 1;
	}
	
	ActiveProjectileClass = ProjectileClasses[NewIndex];

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwitchProjectileSound, GetComponentLocation());

	if (ProjectileOrder.Num() >= NewIndex - 1)
	{
		OnProjectileColorChanged.Broadcast(ProjectileOrder[NewIndex]);	
	}
}

bool UColorCasterComponent::AttachWeapon(AProjectColorCharacter* TargetCharacter)
{
	Character = MakeWeakObjectPtr(TargetCharacter);

	// Check that the character is valid, and has no weapon component yet
	if (!Character.IsValid() || Character->GetInstanceComponents().FindItemByClass<UColorCasterComponent>())
	{
		return false;
	}

	if (ProjectileClasses.Num() > 0)
	{
		ActiveProjectileClass = ProjectileClasses[0];
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this,
			                                   &UColorCasterComponent::Fire);

			// Fire eraser
			EnhancedInputComponent->BindAction(FireEraserAction, ETriggerEvent::Triggered, this,
			                                   &UColorCasterComponent::FireEraser);

			// Load projectile
			EnhancedInputComponent->BindAction(LoadProjectileAction, ETriggerEvent::Triggered, this,
			                                   &UColorCasterComponent::LoadProjectile);
		}
	}

	return true;
}

void UColorCasterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ensure we have a character owner
	if (Character.IsValid())
	{
		// remove the input mapping context from the Player Controller
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	// maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}

void UColorCasterComponent::FireColorProjectile(const TSubclassOf<AProjectileBase>& ProjectileClassToFire)
{
	if (!Character.IsValid() || Character->GetController() == nullptr)
	{
		return;
	}

	// can't fire while focusing
	if (Character->IsTryingToFocus())
	{
		return;
	}

	if (ProjectileClassToFire == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("ProjectileClass to fire is null"));
		return;
	}

	// Try and fire a projectile
	if (UWorld* const World = GetWorld(); World != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the projectile at the muzzle
		World->SpawnActor<AProjectileBase>(ProjectileClassToFire, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
