#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProjectColorSaveSettings.generated.h"

UENUM()
enum EDisplayResolution : uint8
{
	Res_1440P UMETA(DisplayName = "2560 x 1440"),
	Res_1080P UMETA(DisplayName = "1920 x 1080"),
	Res_720P UMETA(DisplayName = "1280 x 720"),
};

UCLASS()
class PROJECTCOLOR_API UProjectColorSaveSettings : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float AudioGeneralVolume;

	UPROPERTY(BlueprintReadWrite)
	float AudioMusicVolume;

	UPROPERTY(BlueprintReadWrite)
	float AudioEffectsVolume;

	UPROPERTY(BlueprintReadWrite)
	float GraphicsBrightness;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EDisplayResolution> GraphicsDisplayResolution;

	/**
	 * Modify below to change defaults.
	 */
	static UProjectColorSaveSettings* CreateDefault()
	{
		UProjectColorSaveSettings* Settings = NewObject<UProjectColorSaveSettings>();
		Settings->AudioGeneralVolume = 1.0f;
		Settings->AudioMusicVolume = 1.0f;
		Settings->AudioEffectsVolume = 1.0f;
		Settings->GraphicsBrightness = 11.0f;
		Settings->GraphicsDisplayResolution = Res_1080P;
		return Settings;
	}
};
