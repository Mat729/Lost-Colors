#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "RateLimiter.generated.h"

UCLASS()
class PROJECTCOLOR_API URateLimiter : public UInputModifier
{
	GENERATED_BODY()

public:
	// seconds between each input execution
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Settings, Config)
	float Seconds = 0.5f;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
	                                                   FInputActionValue CurrentValue, float DeltaTime) override;

private:
	double LastInputTime = 0.0;
};
