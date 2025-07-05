#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "ClimbComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTCOLOR_API UClimbComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:
	UClimbComponent();

	virtual void Interact(AProjectColorCharacter* Interactor) override;

	virtual FText GetInteractionHint() override;

private:
	void Climb(AProjectColorCharacter* Interactor);
};
