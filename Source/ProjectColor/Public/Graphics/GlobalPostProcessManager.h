#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalPostProcessManager.generated.h"

UCLASS()
class PROJECTCOLOR_API AGlobalPostProcessManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color | Post Process", meta = (AllowPrivateAccess = "true"))
	float DefaultBrightness = 10.f;

public:
	AGlobalPostProcessManager();

	inline static const FName GlobalPostProcessName = FName("PCManagedPostProcessVolume");

protected:
	virtual void BeginPlay() override;

private:
	void SpawnPostProcessVolume();
};
