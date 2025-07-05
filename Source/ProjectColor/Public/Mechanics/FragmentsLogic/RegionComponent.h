#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Mechanics/Shooting/Colorable.h"
#include "RegionComponent.generated.h"

class UFragmentComponent;

UENUM(BlueprintType)
enum class ERegionColor : uint8
{
	RColor_Cyan = 0			UMETA(DisplayName = "Cyan"),
	RColor_Magenta = 1		UMETA(DisplayName = "Magenta"),
	RColor_Yellow = 2		UMETA(DisplayName = "Yellow"),
	RColor_Red = 3			UMETA(DisplayName = "Red"),
	RColor_Green = 4		UMETA(DisplayName = "Green"),
	RColor_Blue = 5			UMETA(DisplayName = "Blue"),
	RColor_None = 6			UMETA(Hidden)
};

UCLASS(Blueprintable, ClassGroup="Project Color", meta=(BlueprintSpawnableComponent))
class PROJECTCOLOR_API URegionComponent : public UStaticMeshComponent, public IColorable
{
	GENERATED_BODY()

public:
	URegionComponent();

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void ColorObject(const EProjectileColor ProjectileColor) override;

private:
	TWeakObjectPtr<UFragmentComponent> ParentFragment = nullptr;

	UPROPERTY()
	UMaterialInterface* InitialMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Region Component", meta = (AllowPrivateAccess = true))
	ERegionColor RightColor = ERegionColor::RColor_Cyan;

	ERegionColor CurrentColor = ERegionColor::RColor_None;

	// MATERIALS
	UPROPERTY(EditDefaultsOnly, Category = "Project Color|Materials", meta = (AllowPrivateAccess = true))
	UMaterialInterface* CyanMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Project Color|Materials", meta = (AllowPrivateAccess = true))
	UMaterialInterface* MagentaMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Project Color|Materials", meta = (AllowPrivateAccess = true))
	UMaterialInterface* YellowMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Project Color|Materials", meta = (AllowPrivateAccess = true))
	UMaterialInterface* RedMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Project Color|Materials", meta = (AllowPrivateAccess = true))
	UMaterialInterface* GreenMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Project Color|Materials", meta = (AllowPrivateAccess = true))
	UMaterialInterface* BlueMaterial = nullptr;
	// end MATERIALS

	void ColorRegion(const EProjectileColor ProjectileColor);
	void MixColors(const EProjectileColor ProjectileColor);
	void EraseColors();

	ERegionColor GetRegionColorFromProjectileColor(const EProjectileColor ProjectileColor) const;
	ERegionColor GetMixedColor(const ERegionColor AddedColor) const;
	UMaterialInterface* GetMaterialFromEnumColor(const ERegionColor EnumColor) const;

};
	