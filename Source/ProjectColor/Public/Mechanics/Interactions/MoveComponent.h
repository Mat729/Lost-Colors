#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTCOLOR_API UMoveComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:
	UMoveComponent();

	virtual void Interact(AProjectColorCharacter* Interactor) override;

	virtual FText GetInteractionHint() override;
	
private:
	void Move();
};
