#include "UI/Widgets/InteractionLabelWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Mechanics/Interactions/InteractableComponent.h"
#include "Mechanics/Interactions/InteractionObserverComponent.h"
#include "ProjectColor/ProjectColorCharacter.h"

void UInteractionLabelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetObserverBindings();
}

void UInteractionLabelWidget::NativeDestruct()
{
	ResetObserverBindings();
	
	Super::NativeDestruct();
}

void UInteractionLabelWidget::SetObserverBindings()
{
	AProjectColorCharacter* PlayerCharacter =
		Cast<AProjectColorCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (IsValid(PlayerCharacter))
	{
		InteractionObserverComponent = MakeWeakObjectPtr(
			PlayerCharacter->FindComponentByClass<UInteractionObserverComponent>());
		if (InteractionObserverComponent.IsValid())
		{
			InteractionObserverComponent->OnInteractableComponentChanged.AddDynamic(
				this, &UInteractionLabelWidget::OnInteractableComponentChanged);
		}
	}
}

void UInteractionLabelWidget::ResetObserverBindings()
{
	if (!InteractionObserverComponent.IsValid())
	{
		return;
	}
	InteractionObserverComponent->OnInteractableComponentChanged.RemoveDynamic(
		this, &UInteractionLabelWidget::OnInteractableComponentChanged);
	InteractionObserverComponent.Reset();
}

void UInteractionLabelWidget::OnInteractableComponentChanged(UInteractableComponent* InteractableComponent)
{
	if (!IsValid(InteractableComponent))
	{
		SetText(FText::FromString(""));
		return;
	}
	
	SetText(InteractableComponent->GetInteractionHint());
}


void UInteractionLabelWidget::SetText(const FText& Text) const
{
	TextBlock->SetText(Text);
}
