#include "Mechanics/FragmentsLogic/RegionComponent.h"
#include "Mechanics/FragmentsLogic/FragmentComponent.h"

URegionComponent::URegionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetMobility(EComponentMobility::Movable);
	SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void URegionComponent::BeginPlay()
{
	Super::BeginPlay();

	InitialMaterial = GetMaterial(0);

	UFragmentComponent* Fragment = Cast<UFragmentComponent>(GetAttachParent());
	if (Fragment == nullptr) { return; }
	ParentFragment = MakeWeakObjectPtr(Fragment);
}

void URegionComponent::ColorObject(const EProjectileColor ProjectileColor)
{
	const bool bRegionColored = CurrentColor != ERegionColor::RColor_None;
	const bool bProjectileEraser = ProjectileColor == EProjectileColor::PColor_Eraser;
	
	if (!bRegionColored && !bProjectileEraser)
	{
		ColorRegion(ProjectileColor);
	}
	else if (bRegionColored && !bProjectileEraser)
	{
		MixColors(ProjectileColor);
	}
	else if (bRegionColored && bProjectileEraser)
	{
		EraseColors();
	}
}

void URegionComponent::ColorRegion(const EProjectileColor ProjectileColor)
{
	const bool bWasProperlyColored = CurrentColor == RightColor;
	
	// converte enum del colore proiettile a enum del colore della regione
	CurrentColor = GetRegionColorFromProjectileColor(ProjectileColor);

	// ottiene e setta il nuovo materiale sulla regione
	UMaterialInterface* NewMaterial = GetMaterialFromEnumColor(CurrentColor);
	SetMaterial(0, NewMaterial);

	// verifica se il nuovo colore è quello corretto
	const bool bIsProperlyColored = CurrentColor == RightColor;

	// avvisa il frammento
	if (!ParentFragment.IsValid()) { return; }
	ParentFragment->OnRegionColorChanged(bIsProperlyColored, bWasProperlyColored);
}

void URegionComponent::MixColors(const EProjectileColor AddedProjectileColor)
{
	// converte enum del colore proiettile a enum del colore della regione
	const ERegionColor AddedColor = GetRegionColorFromProjectileColor(AddedProjectileColor);
	
	// ottiene colore secondario
	const ERegionColor MixedColor = GetMixedColor(AddedColor);

	if (CurrentColor == MixedColor) { return; }
	
	const bool bWasProperlyColored = CurrentColor == RightColor;

	CurrentColor = MixedColor;
	// ottiene e setta il nuovo materiale sulla regione
	UMaterialInterface* NewMaterial = GetMaterialFromEnumColor(CurrentColor);
	SetMaterial(0, NewMaterial);

	// verifica se il nuovo colore è quello corretto
	const bool bIsProperlyColored = CurrentColor == RightColor;

	// avvisa il frammento
	if (!ParentFragment.IsValid()) { return; }
	ParentFragment->OnRegionColorChanged(bIsProperlyColored, bWasProperlyColored);
}

void URegionComponent::EraseColors()
{
	const bool bWasProperlyColored = CurrentColor == RightColor;
	
	CurrentColor = ERegionColor::RColor_None;
	SetMaterial(0, InitialMaterial);

	if (!ParentFragment.IsValid()) { return; }
	ParentFragment->OnRegionColorChanged(false, bWasProperlyColored);
}

ERegionColor URegionComponent::GetRegionColorFromProjectileColor(const EProjectileColor ProjectileColor) const
{
	switch (ProjectileColor)
	{
		case EProjectileColor::PColor_Cyan:
			return ERegionColor::RColor_Cyan;
		case EProjectileColor::PColor_Magenta:
			return ERegionColor::RColor_Magenta;
		case EProjectileColor::PColor_Yellow:
			return ERegionColor::RColor_Yellow;
		default:
			return ERegionColor::RColor_Cyan;
	}
}

ERegionColor URegionComponent::GetMixedColor(const ERegionColor AddedColor) const
{
	if (CurrentColor == ERegionColor::RColor_Cyan)
	{
		if (AddedColor == ERegionColor::RColor_Magenta)
		{
			return ERegionColor::RColor_Blue;
		}
		if (AddedColor == ERegionColor::RColor_Yellow)
		{
			return ERegionColor::RColor_Green;
		}
	}
	else if (CurrentColor == ERegionColor::RColor_Magenta)
	{
		if (AddedColor == ERegionColor::RColor_Cyan)
		{
			return ERegionColor::RColor_Blue;
		}
		if (AddedColor == ERegionColor::RColor_Yellow)
		{
			return ERegionColor::RColor_Red;
		}
	}
	else if (CurrentColor == ERegionColor::RColor_Yellow)
	{
		if (AddedColor == ERegionColor::RColor_Cyan)
		{
			return ERegionColor::RColor_Green;
		}
		if (AddedColor == ERegionColor::RColor_Magenta)
		{
			return ERegionColor::RColor_Red;
		}
	}
	
	// se la regione era già colorata con un colore secondario, non viene modificata (?)
	return CurrentColor;
}


UMaterialInterface* URegionComponent::GetMaterialFromEnumColor(const ERegionColor EnumColor) const
{
	switch (EnumColor)
	{
		case ERegionColor::RColor_Cyan:
			return CyanMaterial;
		case ERegionColor::RColor_Magenta:
			return MagentaMaterial;
		case ERegionColor::RColor_Yellow:
			return YellowMaterial;
		case ERegionColor::RColor_Red:
			return RedMaterial;
		case ERegionColor::RColor_Green:
			return GreenMaterial;
		case ERegionColor::RColor_Blue:
			return BlueMaterial;
		default:
			return InitialMaterial;
	}
}
