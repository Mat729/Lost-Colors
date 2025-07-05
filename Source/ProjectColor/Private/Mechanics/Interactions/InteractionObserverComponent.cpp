#include "Mechanics/Interactions/InteractionObserverComponent.h"

UInteractionObserverComponent::UInteractionObserverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionObserverComponent::UpdateCurrentInteractableComponent(UInteractableComponent* CurInteractableComponent)
{
	OnInteractableComponentChanged.Broadcast(CurInteractableComponent);
}
