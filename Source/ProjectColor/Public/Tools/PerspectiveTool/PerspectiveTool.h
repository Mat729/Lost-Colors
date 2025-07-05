#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "PerspectiveTool.generated.h"

class AFragmentedObject;
class ATriggerZone;
class UPerspectiveToolWidget;


UCLASS(Blueprintable, ClassGroup="Project Color|Tools")
class PROJECTCOLOR_API APerspectiveTool : public AInfo
{
	GENERATED_BODY()

public:
	APerspectiveTool();
	
	void SetCorrectPerspective();

	void MoveAndScaleFragment(const float InDistance);
	
	void MoveToCorrectView() const;

	void OnPerspectificationCompleted();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Project Color|Perspective Tool", meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<AFragmentedObject> FragmentedObject = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Project Color|Perspective Tool", meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<ATriggerZone> TriggerZone = nullptr;

	TWeakObjectPtr<ACharacter> PlayerCharacter = nullptr;
	TWeakObjectPtr<APlayerCameraManager> PlayerCameraManager = nullptr;
	TWeakObjectPtr<APlayerController> PlayerController = nullptr;

	// UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Project Color|Perspective Tool", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPerspectiveToolWidget> PerspectiveToolWidgetClass;
	UPROPERTY()
	TObjectPtr<UPerspectiveToolWidget> PerspectiveToolWidget = nullptr;
	// fine UI

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Perspective Tool", meta = (AllowPrivateAccess = true))
	float MinDistanceFromCamera = 120.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Perspective Tool", meta = (AllowPrivateAccess = true))
	float MaxDistanceFromCamera = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Perspective Tool", meta = (AllowPrivateAccess = true))
	UMaterialInterface* SelectionOverlayMaterial = nullptr;

	float InitialDistance = 0.0f;
	const float InitialScale = 1.0f;

	FVector PlayerLocation = FVector::ZeroVector;
	FVector CameraLocation = FVector::ZeroVector;
	FRotator CameraRotation = FRotator::ZeroRotator;
	FVector CameraDirection = FVector::ZeroVector;

	TWeakObjectPtr<USceneComponent> SelectedFragment = nullptr;
	
	bool bPerspectiveSet = false;
	bool bMovedFragment = false;
	
	void SetUpUI();
	
	void CheckDistance();

	UFUNCTION(BlueprintCallable, Category = "Project Color|Perspective Tool", meta = (AllowPrivateAccess = true))
	void OnMouseClick();
	
	void SelectFragment(USceneComponent* InSelectedComponent);
	
	void DeselectFragment();

	void SetOverlayMaterialOnFragment(UMaterialInterface* InOverlayMaterial) const;
	
};
