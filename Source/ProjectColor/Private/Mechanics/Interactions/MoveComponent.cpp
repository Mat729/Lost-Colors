#include "Mechanics/Interactions/MoveComponent.h"

UMoveComponent::UMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMoveComponent::Interact(AProjectColorCharacter* Interactor)
{
	Super::Interact(Interactor);

	Move();
}

FText UMoveComponent::GetInteractionHint()
{
	const FText KeyText = FText::FromName(InteractionBtnKey.GetFName());
	return FText::Format(NSLOCTEXT("Game", "ClimbObject", "Press {0} to move the object"), KeyText);
}

void UMoveComponent::Move()
{
	// todo: do Move
	
	OnInteractionCompleted();
}
