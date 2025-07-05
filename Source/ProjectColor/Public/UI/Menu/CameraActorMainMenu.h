#pragma once

#include "Camera/CameraActor.h"
#include "CameraActorMainMenu.generated.h"

class USplineComponent;

UCLASS(config=Game, showcategories=(Input))
class PROJECTCOLOR_API ACameraActorMainMenu : public ACameraActor
{
	GENERATED_BODY()

public:
	ACameraActorMainMenu();
	
private:
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess=true))
	TObjectPtr<USplineComponent> CameraMovementPath = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectColor", meta = (AllowPrivateAccess=true))
	float CameraMovementTime = 2.f;

	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "ProjectColor")
	void StartMovement();
	
	UFUNCTION(BlueprintCallable, Category = "ProjectColor")
	void CameraMovement(float InAlpha) const;
};
