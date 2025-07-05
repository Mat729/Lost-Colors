#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ColorCasterComponent.generated.h"

enum class EProjectileColor : uint8;
class UInputAction;
class AColorProjectile;
class AEraserProjectile;
struct FInputActionValue;
class AProjectColorCharacter;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileColorChanged, EProjectileColor, ProjectileColor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTCOLOR_API UColorCasterComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UColorCasterComponent();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TArray<TSubclassOf<AColorProjectile>> ProjectileClasses;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TArray<EProjectileColor> ProjectileOrder;

	/** Eraser projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AColorProjectile> EraserProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireEraserAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LoadProjectileAction;

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(AProjectColorCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireEraser();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void LoadProjectile(const FInputActionValue& ActionValue);

	UPROPERTY(BlueprintAssignable)
	FOnProjectileColorChanged OnProjectileColorChanged;

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// the Character holding this weapon
	TWeakObjectPtr<AProjectColorCharacter> Character;

	// the current active projectile class
	TSubclassOf<AColorProjectile> ActiveProjectileClass;
	
	void FireColorProjectile(const TSubclassOf<class AProjectileBase>& ProjectileClassToFire);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Project Color|Sound", meta = (AllowPrivateAccess = true))
	USoundBase* SwitchProjectileSound = nullptr;
};
