#include "Input/Modifiers/RateLimiter.h"

FInputActionValue URateLimiter::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                         const FInputActionValue CurrentValue, const float DeltaTime)
{
	const double CurrentTime = FPlatformTime::Seconds();

	// ignore the input if the elapsed time is not enough
	if (CurrentTime - LastInputTime < Seconds)
	{
		return Super::ModifyRaw_Implementation(PlayerInput, 0.f, DeltaTime);
	}

	LastInputTime = CurrentTime;

	return Super::ModifyRaw_Implementation(PlayerInput, CurrentValue, DeltaTime);
}
