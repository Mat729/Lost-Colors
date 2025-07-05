#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FragmentComponent.generated.h"


class AFragmentedObject;

UCLASS(Blueprintable, ClassGroup="Project Color", meta=(BlueprintSpawnableComponent))
class PROJECTCOLOR_API UFragmentComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UFragmentComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TWeakObjectPtr<AFragmentedObject> FragmentedObject = nullptr;

// regions
	int32 RegionsNumber = 0;
	int32 RegionsReadyNumber = 0;
	bool bWasReady = false;
// regions

// floating
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Floating", meta = (AllowPrivateAccess = true, Units = "Seconds", UIMin = 0, ClampMin = 0))
	float FloatingTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Floating", meta = (AllowPrivateAccess = true, Units = "Centimeters", UIMin = 0, ClampMin = 0))
	float FloatingAmplitude = 16.0f;
	
	FVector InitialLocation = FVector::ZeroVector;
	float MinZ = 0;
	float MaxZ = 0;
	float Speed = 0;
	float Alpha = 0;
	bool bFloating = false;
	
	void StartFloatingAfterDelay();
	void StartFloating();
	void UpdateFloating(const float InDeltaTime);
	// floating

public:
	// funzione chiamata dalle regions sul fragment
	UFUNCTION(BlueprintCallable, Category = "Project Color|Fragment Component")
	void OnRegionColorChanged(const bool bIsProperlyColored, const bool bWasProperlyColored);

	void StopFloating();
	
private:
	//funzione di update dello stato del frammento
	void OnRegionUpdate() const;

};
