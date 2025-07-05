#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectileColorUtils.generated.h"

enum class EProjectileColor : uint8;

UCLASS()
class PROJECTCOLOR_API UProjectileColorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Project Color | Projectile Color")
	static FLinearColor ToLinearColor(EProjectileColor Color);
};
