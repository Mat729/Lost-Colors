#include "ProjectColorGameInstance.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UProjectColorGameInstance::PlayUIMusic()
{
	const bool bFadingOut = StartFadeOut(GameMusicPlaying);

	if (UIMusicPlaying && UIMusicPlaying->IsPlaying())
	{
		UIMusicPlaying->Stop();
	}

	UIMusicPlaying = UGameplayStatics::SpawnSound2D(GetWorld(), UIMusic, 1.0f, 1.0f, 0, nullptr, true, false);
	if (UIMusicPlaying == nullptr) { return; }
	
	if (bFadingOut && Delay > 0.0f)
	{
		FadeInAfterDelay(UIMusicPlaying);
	}
	else
	{
		StartFadeIn(UIMusicPlaying);
	}
}

void UProjectColorGameInstance::PlayGameMusic()
{
	const bool bFadingOut = StartFadeOut(UIMusicPlaying);

	if (GameMusicPlaying && GameMusicPlaying->IsPlaying())
	{
		GameMusicPlaying->Stop();
	}

	GameMusicPlaying = UGameplayStatics::SpawnSound2D(GetWorld(), GameMusic, 1.0f, 1.0f, 0, nullptr, true, false);
	if (GameMusicPlaying == nullptr) { return; }
	
	if (bFadingOut && Delay > 0.0f)
	{
		FadeInAfterDelay(GameMusicPlaying);
	}
	else
	{
		StartFadeIn(GameMusicPlaying);
	}
}

void UProjectColorGameInstance::StartFadeIn(UAudioComponent* InMusic) const
{
	if (InMusic == nullptr) { return; }
	if (InMusic->IsPlaying()) { UE_LOG(LogTemp, Warning, TEXT("InMusic->IsPlaying")); }
	if (InMusic->bIsPaused) { UE_LOG(LogTemp, Warning, TEXT("InMusic->bIsPaused")); }

	InMusic->FadeIn(FadeInDuration, 1.0f, 0, EAudioFaderCurve::Sin);
}

bool UProjectColorGameInstance::StartFadeOut(UAudioComponent* InMusic) const
{
	if (InMusic == nullptr) { return false; }
	if (!InMusic->IsPlaying()) { return false; }

	InMusic->FadeOut(FadeOutDuration, 0, EAudioFaderCurve::Sin);
	return true;
}
