#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaintSplatterWidget.generated.h"

class UImage;

UCLASS(Blueprintable, ClassGroup="Project Color")
class PROJECTCOLOR_API UPaintSplatterWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "Project Color|Paint Splatter Widget", meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UImage> PaintSplatterImage = nullptr;

	// scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project Color|Paint Splatter Widget", meta = (AllowPrivateAccess = true, ClampMin = "0.1", ClampMax = "1"))
	float MinScale = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project Color|Paint Splatter Widget", meta = (AllowPrivateAccess = true, ClampMin = "0.1", ClampMax = "1"))
	float MaxScale = 1.0f;

	// colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project Color|Paint Splatter Widget", meta = (AllowPrivateAccess = true))
	TArray<FLinearColor> Colors;

	// Lifespan
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project Color|Paint Splatter Widget", meta = (AllowPrivateAccess = true, ClampMin = "0.1"))
	float MinLifespan = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Project Color|Paint Splatter Widget", meta = (AllowPrivateAccess = true, ClampMin = "0.1"))
	float MaxLifespan = 6.0f;

public:
	UFUNCTION()
	void SetPaintSplatter();
};
