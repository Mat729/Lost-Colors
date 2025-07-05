#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectColorGameInstance.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS()
class PROJECTCOLOR_API UProjectColorGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Music", meta = (AllowPrivateAccess = true, Units = "Seconds", UIMin = 0, ClampMin = 0))
	float FadeInDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Music", meta = (AllowPrivateAccess = true, Units = "Seconds", UIMin = 0, ClampMin = 0))
	float FadeOutDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Music", meta = (AllowPrivateAccess = true, Units = "Seconds", UIMin = 0, ClampMin = 0))
	float Delay = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Music", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> UIMusic = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Music", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> GameMusic = nullptr;

	UPROPERTY()
	UAudioComponent* UIMusicPlaying = nullptr;

	UPROPERTY()
	UAudioComponent* GameMusicPlaying = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "Project Color|Music")
	void PlayUIMusic();

	UFUNCTION(BlueprintCallable, Category = "Project Color|Music")
	void PlayGameMusic();

	UFUNCTION(BlueprintImplementableEvent, Category = "Project Color|Music")
	void FadeInAfterDelay(UAudioComponent* InMusic);

private:
	UFUNCTION(BlueprintCallable, Category = "Project Color|Music")
	void StartFadeIn(UAudioComponent* InMusic) const;

	bool StartFadeOut(UAudioComponent* InMusic) const;
};
