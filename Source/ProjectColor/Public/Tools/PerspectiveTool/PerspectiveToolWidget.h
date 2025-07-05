#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerspectiveToolWidget.generated.h"

class USlider;
class UButton;
class UTextBlock;
class UOverlay;
class APerspectiveTool;

UCLASS()
class PROJECTCOLOR_API UPerspectiveToolWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<USlider> DistanceSlider = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> SetPerspectiveBtn = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> MoveToPerspectiveBtn = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> DoneBtn = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> PerspectiveSetText = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DistanceValueText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UOverlay> DoneOverlay = nullptr;
	
	void SetPerspectiveTool(APerspectiveTool* InPerspectiveTool);

	void SetSliderValues(float InMinValue, float InMaxValue, float InCurrentValue);
	void EnableSlider(bool bInIsEnabled, float InValue);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	TWeakObjectPtr<APerspectiveTool> PerspectiveTool = nullptr;

	bool bPerspectiveSet = false;

	UFUNCTION()
	void OnDistanceSliderValueChanged(const float InValue);
	
	UFUNCTION()
	void OnSetPerspectiveClicked();

	UFUNCTION()
	void OnMoveToPerspectiveClicked();

	UFUNCTION()
	void OnDoneBtnClicked();
};
