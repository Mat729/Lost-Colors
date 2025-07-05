#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionLabelWidget.generated.h"

class UInteractionObserverComponent;
class UInteractableComponent;
class UTextBlock;

UCLASS()
class PROJECTCOLOR_API UInteractionLabelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock = nullptr;

private:
	void SetObserverBindings();
	void ResetObserverBindings();
	void SetText(const FText& Text) const;

	UFUNCTION()
	void OnInteractableComponentChanged(UInteractableComponent* InteractableComponent);

	TWeakObjectPtr<UInteractionObserverComponent> InteractionObserverComponent;
};