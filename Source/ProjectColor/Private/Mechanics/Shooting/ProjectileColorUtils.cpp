#include "Mechanics/Shooting/ProjectileColorUtils.h"

#include "Mechanics/Shooting/Projectiles/ColorProjectile.h"

FLinearColor UProjectileColorUtils::ToLinearColor(EProjectileColor Color)
{
	switch (Color)
	{
	case EProjectileColor::PColor_Cyan:
		return FLinearColor(0.0f, 0.75f, 0.75f, 1.0f);
	case EProjectileColor::PColor_Magenta:
		return FLinearColor(0.75f, 0.0f, 0.75f, 1.0f);
	case EProjectileColor::PColor_Yellow:
		return FLinearColor(0.75f, 0.75f, 0.0f, 1.0f);
	default:
	case EProjectileColor::PColor_Eraser:
		return FLinearColor(0.25f, 0.25f, 0.25f, 1.0f);
	}
}
