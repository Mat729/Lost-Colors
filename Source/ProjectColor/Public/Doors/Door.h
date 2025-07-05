#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class PROJECTCOLOR_API ADoor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color | Door", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color | Door", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = nullptr;

public:
	ADoor();

	UPROPERTY(EditAnywhere, Category = "Project Color | Door")
	TObjectPtr<class AFragmentedObject> FragmentedObject = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Project Color | Door")
	void OpenDoor();

private:
	UFUNCTION()
	void OnFragmentPuzzleResolved();
};
