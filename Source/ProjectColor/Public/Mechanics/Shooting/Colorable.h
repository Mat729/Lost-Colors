#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ColorProjectile.h"
#include "UObject/Interface.h"
#include "Colorable.generated.h"

UINTERFACE(NotBlueprintable)
class UColorable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTCOLOR_API IColorable
{
	GENERATED_BODY()

public:
	virtual void ColorObject(EProjectileColor Color) = 0;
};
