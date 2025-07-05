#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GraphicsUtils.generated.h"

UCLASS()
class PROJECTCOLOR_API UGraphicsUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Project Color | Utils | Post Process", meta = (WorldContext = "WorldContextObject"))
	static APostProcessVolume* FindGlobalPostProcessVolume(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Project Color | Utils | Post Process", meta = (WorldContext = "WorldContextObject"))
	static void SetGlobalBrightness(const UObject* WorldContextObject, const float Brightness);

	UFUNCTION(BlueprintCallable, Category = "Project Color | Utils | Post Process", meta = (WorldContext = "WorldContextObject"))
	static float GetGlobalBrightness(const UObject* WorldContextObject);

private:
	static void UpdateCameraPostProcessSettings(const UObject* WorldContextObject, const float AutoExposureBias);
};
