#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerZone.generated.h"

class AProjectColorCharacter;
class USphereComponent;
class AFragmentedObject;
class UCameraAlignmentComponent;

UCLASS()
class PROJECTCOLOR_API ATriggerZone : public AActor
{
	GENERATED_BODY()

public:
	ATriggerZone();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Project Color|Trigger Zone", meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> TriggerArea;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bPlayerInside = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Trigger Zone", meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<AFragmentedObject> FragmentedObject = nullptr;

	TWeakObjectPtr<UCameraAlignmentComponent> CameraAlignment = nullptr;
	
	TWeakObjectPtr<AProjectColorCharacter> PlayerCharacter = nullptr;

public:
	// metodo pubblico chiamato dal prospettivizzatore in fase di creazione del livello
	UFUNCTION(BlueprintCallable, Category = "Project Color|Trigger Zone")
	void SetPointOfViewLocation(const FVector InLocation);
};
