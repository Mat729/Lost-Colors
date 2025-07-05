#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "Projectiles/ColorProjectile.h"
#include "SaveSystem/GameSavable.h"
#include "ColorCasterPickup.generated.h"

class AProjectColorCharacter;
class UEquipWeaponComponent;
class UColorCasterComponent;

USTRUCT(BlueprintType)
struct FPickableColorCasterState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bIsPickedUp;
};

UCLASS()
class PROJECTCOLOR_API AColorCasterPickup : public AActor, public IGameSavable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color | Color Caster", meta = (AllowPrivateAccess = true))
	TObjectPtr<UColorCasterComponent> ColorCasterComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color | Color Caster", meta = (AllowPrivateAccess = true))
	TObjectPtr<UEquipWeaponComponent> EquipWeaponComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color | Color Caster", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> BrushPaintEffect = nullptr;

	/** Holds the explicit picking state of the color caster */
	bool bIsPickedUp = false;

	const FString& SaveBlockId = FString("ColorCaster");
public:
	AColorCasterPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color | Color Caster")
	TObjectPtr<UNiagaraSystem> BrushPaintSystem = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePaintBrushTip(EProjectileColor ProjectileColor);

protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual FGameStateBlock FetchState() const override;

	virtual void ApplyState(const FGameStateBlock& Block) override;

private:
	UFUNCTION()
	void OnWeaponPickUp(AProjectColorCharacter* PickUpCharacter);
	
	UFUNCTION()
	void OnWeaponProjectileChanged(EProjectileColor ProjectileColor);

	void UpdateBrushPaintVfxColor(EProjectileColor ProjectileColor);
	
};