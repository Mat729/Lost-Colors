#include "Mechanics/Interactions/ClimbComponent.h"

UClimbComponent::UClimbComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UClimbComponent::Interact(AProjectColorCharacter* Interactor)
{
	Super::Interact(Interactor);

	Climb(Interactor);
}

FText UClimbComponent::GetInteractionHint()
{
	const FText KeyText = FText::FromName(InteractionBtnKey.GetFName());
	return FText::Format(NSLOCTEXT("Game", "ClimbObject", "Press {0} to climb the object"), KeyText);
}

void UClimbComponent::Climb(AProjectColorCharacter* Interactor)
{
	// todo: do Climb

	OnInteractionCompleted();
}
