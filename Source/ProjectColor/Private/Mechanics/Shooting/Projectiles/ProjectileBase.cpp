#include "Mechanics/Shooting/Projectiles/ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Mechanics/FragmentsLogic/RegionComponent.h"
#include "Mechanics/Shooting/ProjectileColorUtils.h"

AProjectileBase::AProjectileBase() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

bool AProjectileBase::ShouldSpawnDecal(bool bColorableObjectHit)
{
	if (bColorableObjectHit)
	{
		return !bDecalsOnGenericObjectsOnly;
	}
	return true;
}

void AProjectileBase::GenerateDecal(const FHitResult& Hit, const FLinearColor& DecalColor)
{
	// create dynamic instance of decal material
	UMaterialInstanceDynamic* DecalDynamicMaterial = UMaterialInstanceDynamic::Create(DecalMaterial, this);
	if (!IsValid(DecalDynamicMaterial)) { return; }

	// set decal color
	DecalDynamicMaterial->SetVectorParameterValue(DecalMaterialColorParameterName, DecalColor);

	// set random texture
	if (!DecalTextures.IsEmpty())
	{
		const int32 RandomIndex = FMath::RandRange(0, DecalTextures.Num() - 1);
		UTexture* DecalTexture = DecalTextures[RandomIndex];
		DecalDynamicMaterial->SetTextureParameterValue(DecalMaterialTextureParameterName, DecalTexture);
	}

	// set decal size
	const float DecalSizeFloat = FMath::RandRange(DecalMinSize, DecalMaxSize);
	const FVector DecalSize = DecalSizeFloat * FVector::One();

	// location = hit impact point
	const FVector SpawnLocation = Hit.ImpactPoint;

	// rotation
	const float RandomAngle = FMath::RandRange(0.0f, 360.0f);
	const FRotator HitRotation = Hit.Normal.Rotation();
	const FRotator SpawnRotation = FRotator(HitRotation.Pitch, HitRotation.Yaw, RandomAngle);

	// lifespan
	const float DecalLifespan = FMath::RandRange(DecalMinLifespan, DecalMaxLifespan);

	// spawn decal
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalDynamicMaterial, DecalSize, SpawnLocation, SpawnRotation, DecalLifespan);
}
