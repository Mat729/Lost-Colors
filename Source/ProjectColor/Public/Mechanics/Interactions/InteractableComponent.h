#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "InteractableComponent.generated.h"

class AProjectColorCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionComplete);

UCLASS(Abstract, NotBlueprintable)
class PROJECTCOLOR_API UInteractableComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UInteractableComponent();

	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;

	virtual void UninitializeComponent() override;

	UFUNCTION(BlueprintCallable)
	virtual FText GetInteractionHint();

	UPROPERTY()
	FOnInteractionComplete OnInteractionComplete;

protected:
	UPROPERTY(EditAnywhere, Category = "Project Color|Interactable Component")
	FKey InteractionBtnKey = EKeys::E;
	
	UFUNCTION()
	void OnInteractionAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void Interact(AProjectColorCharacter* Interactor);
	
	UFUNCTION()
	void OnInteractionCompleted() const;

	UFUNCTION()
	void EnableInteraction();
	
	UFUNCTION()
	void DisableInteraction();
	
	bool bCanInteract = false;
	bool bInputKeyBound = false;

private:
	UFUNCTION()
	void OnInteractionStarted();

	void NotifyInteractionObserver(UInteractableComponent* InteractableComponent) const;
	
	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<AProjectColorCharacter> InteractingPlayer = nullptr;
};
