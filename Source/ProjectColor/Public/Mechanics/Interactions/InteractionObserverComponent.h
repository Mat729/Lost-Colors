#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionObserverComponent.generated.h"

class UInteractableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableComponentChanged, UInteractableComponent*,
                                            InteractableComponent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTCOLOR_API UInteractionObserverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionObserverComponent();

	void UpdateCurrentInteractableComponent(UInteractableComponent* CurInteractableComponent);

	UPROPERTY()
	FOnInteractableComponentChanged OnInteractableComponentChanged;
};
