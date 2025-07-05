#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS(config=Game)
class PROJECTCOLOR_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Project Color|Paint Splatter Decal")
	bool bDecalsOnGenericObjectsOnly = false;
	
	// decal material and textures
	UPROPERTY(EditDefaultsOnly, Category="Project Color|Paint Splatter Decal")
	TObjectPtr<UMaterialInterface> DecalMaterial = nullptr;

	FName DecalMaterialColorParameterName = FName(TEXT("Color"));
	FName DecalMaterialTextureParameterName = FName(TEXT("Texture"));

	UPROPERTY(EditDefaultsOnly, Category="Project Color|Paint Splatter Decal")
	TArray<TObjectPtr<UTexture>> DecalTextures;
	
	// decal size
	UPROPERTY(EditDefaultsOnly, Category="Project Color|Paint Splatter Decal", meta = (ClampMin = "0.1"))
	float DecalMinSize = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category="Project Color|Paint Splatter Decal", meta = (ClampMin = "0.1"))
	float DecalMaxSize = 80.0f;

	// decal life span
	UPROPERTY(EditDefaultsOnly, Category="Project Color|Paint Splatter Decal", meta = (ClampMin = "0.1"))
	float DecalMinLifespan = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category="Project Color|Paint Splatter Decal", meta = (ClampMin = "0.1"))
	float DecalMaxLifespan = 10.0f;

	void GenerateDecal(const FHitResult& Hit, const FLinearColor& DecalColor);

	bool ShouldSpawnDecal(bool bColorableObjectHit);

public:
	AProjectileBase();

	/** called when projectile hits something */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

};
