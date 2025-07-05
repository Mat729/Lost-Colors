#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "CameraAlignmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCameraAlignedEvent);

UCLASS(Blueprintable, ClassGroup="Project Color", meta=(BlueprintSpawnableComponent))
class PROJECTCOLOR_API UCameraAlignmentComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	UCameraAlignmentComponent();

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<APlayerCameraManager> PlayerCameraManager = nullptr;

	FVector CorrectForward = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Camera Alignment", meta = (AllowPrivateAccess = true))
	float CameraRotationThreshold = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Camera Alignment", meta = (AllowPrivateAccess = true))
	bool bShowDebugMessages = false;
	
public:
	// metodo pubblico chiamato dal prospettivizzatore in fase di creazione del livello
	UFUNCTION(BlueprintCallable, Category = "Project Color|Camera Alignment")
	void SetCorrectViewDirection(const FRotator InCorrectCameraRotation);
	
	// metodo pubblico chiamato dalla trigger zone
	UFUNCTION(BlueprintCallable, Category = "Project Color|Camera Alignment")
	void CheckCameraAlignment();

	UPROPERTY()
	FCameraAlignedEvent OnCameraAligned;
};
