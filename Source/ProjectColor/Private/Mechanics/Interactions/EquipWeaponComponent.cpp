#include "Mechanics/Interactions/EquipWeaponComponent.h"

#include "Mechanics/Shooting/ColorCasterComponent.h"

UEquipWeaponComponent::UEquipWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipWeaponComponent::Interact(AProjectColorCharacter* Interactor)
{
	Super::Interact(Interactor);

	Equip(Interactor);
}

FText UEquipWeaponComponent::GetInteractionHint()
{
	const FText KeyText = FText::FromName(InteractionBtnKey.GetFName());
	return FText::Format(NSLOCTEXT("Game", "EquipWeapon", "Press {0} to pick up the brush"), KeyText);
}

void UEquipWeaponComponent::Equip(AProjectColorCharacter* Interactor)
{
	if (UActorComponent* Component = GetOwner()->GetComponentByClass(UColorCasterComponent::StaticClass());
		Component != nullptr)
	{
		UColorCasterComponent* ColorCasterComponent = Cast<UColorCasterComponent>(Component);
		if (ColorCasterComponent->AttachWeapon(Interactor))
		{
			OnPickUp.Broadcast(Interactor);
		}
	}

	OnInteractionCompleted();

	DisableInteraction();
}
