#include "Mechanics/Shooting/Projectiles/ColorProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Mechanics/Shooting/Colorable.h"
#include "Mechanics/Shooting/ProjectileColorUtils.h"

AColorProjectile::AColorProjectile()
{
	// PrimaryComponentTick.bCanEverTick = true
}

void AColorProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
	
	bool bColored = ColorObjectIfColorable(OtherComp);

	if (ShouldSpawnDecal(bColored))
	{
		const FLinearColor DecalColor = UProjectileColorUtils::ToLinearColor(ProjectileColor);
		GenerateDecal(Hit, DecalColor);
	}

	Destroy();
}

bool AColorProjectile::ColorObjectIfColorable(UPrimitiveComponent* OtherComp)
{
	IColorable *ColorableObject = Cast<IColorable>(OtherComp);
	
	// not a colorable object, bail out
	if (ColorableObject == nullptr) return false;
	
	ColorableObject->ColorObject(ProjectileColor);

	return true;
}
