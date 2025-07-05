#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "EquipWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AProjectColorCharacter*, PickUpCharacter);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTCOLOR_API UEquipWeaponComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:
	UEquipWeaponComponent();

	UPROPERTY(BlueprintAssignable)
	FOnPickUp OnPickUp;

	virtual void Interact(AProjectColorCharacter* Interactor) override;

	virtual FText GetInteractionHint() override;
	
	void Equip(AProjectColorCharacter* Interactor);
};
