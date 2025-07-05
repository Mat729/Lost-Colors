#pragma once

#include "Blueprint/UserWidget.h"
#include "GamePageBase.generated.h"

class UPaintSplatterWidget;
class USoundBase;

UCLASS(Abstract)
class PROJECTCOLOR_API UGamePageBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Paint Splatter Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPaintSplatterWidget> PaintSplatterWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Project Color|Paint Splatter Widget", meta = (AllowPrivateAccess = true))
	USoundBase* PaintSplatterSound = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Project Color|Paint Splatter Widget")
	void SpawnPaintSplatter();
};
