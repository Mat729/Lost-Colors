#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "GameFramework/Actor.h"
#include "ColorProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class USoundBase;

UENUM(BlueprintType)
enum class EProjectileColor : uint8
{
	PColor_Cyan = 0		UMETA(DisplayName = "Cyan"),
	PColor_Magenta = 1	UMETA(DisplayName = "Magenta"),
	PColor_Yellow = 2		UMETA(DisplayName = "Yellow"),
	PColor_Eraser = 3	UMETA(DisplayName = "Eraser"),
};

UCLASS(config=Game)
class PROJECTCOLOR_API AColorProjectile : public AProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Project Color|Projectile")
	EProjectileColor ProjectileColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Project Color|Projectile")
	USoundBase* ImpactSound = nullptr;

public:
	AColorProjectile();

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	// returns whether or not the object has been colored 
	bool ColorObjectIfColorable(UPrimitiveComponent* OtherComp);

};
