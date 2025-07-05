#include "Tools/PerspectiveTool/PerspectiveToolWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Tools/PerspectiveTool/PerspectiveTool.h"


void UPerspectiveToolWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (DistanceSlider != nullptr)
	{
		DistanceSlider->OnValueChanged.AddDynamic(this, &UPerspectiveToolWidget::OnDistanceSliderValueChanged);
	}

	if (SetPerspectiveBtn != nullptr)
	{
		SetPerspectiveBtn->OnClicked.AddDynamic(this, &UPerspectiveToolWidget::OnSetPerspectiveClicked);
	}

	if (MoveToPerspectiveBtn != nullptr)
	{
		MoveToPerspectiveBtn->OnClicked.AddDynamic(this, &UPerspectiveToolWidget::OnMoveToPerspectiveClicked);
	}

	if (DoneBtn != nullptr)
	{
		DoneBtn->OnClicked.AddDynamic(this, &UPerspectiveToolWidget::OnDoneBtnClicked);
	}
}

void UPerspectiveToolWidget::SetPerspectiveTool(APerspectiveTool* InPerspectiveTool)
{
	if (InPerspectiveTool != nullptr)
	{
		PerspectiveTool = MakeWeakObjectPtr(InPerspectiveTool);
	}
}

void UPerspectiveToolWidget::SetSliderValues(float InMinValue, float InMaxValue, float InCurrentValue)
{
	if (DistanceSlider == nullptr) { return; }
	DistanceSlider->SetMinValue(InMinValue);
	DistanceSlider->SetMaxValue(InMaxValue);
	DistanceSlider->SetValue(InCurrentValue);
}

void UPerspectiveToolWidget::EnableSlider(bool bInIsEnabled, float InValue)
{
	if (DistanceSlider == nullptr) { return; }
	DistanceSlider->SetIsEnabled(bInIsEnabled);
	DistanceSlider->SetValue(InValue);
}

void UPerspectiveToolWidget::OnDistanceSliderValueChanged(const float InValue)
{
	if (!bPerspectiveSet) { return;}
	
	if (!PerspectiveTool.IsValid()) { return; }
	PerspectiveTool->MoveAndScaleFragment(InValue);

	if (DistanceValueText != nullptr)
	{
		DistanceValueText->SetText(FText::FromString(FString::SanitizeFloat(InValue)));
	}

	if (SetPerspectiveBtn != nullptr)
	{
		SetPerspectiveBtn->SetIsEnabled(false);
	}
}

void UPerspectiveToolWidget::OnSetPerspectiveClicked()
{	
	if (!PerspectiveTool.IsValid()) { return; }
	PerspectiveTool->SetCorrectPerspective();

	bPerspectiveSet = true;

	if (PerspectiveSetText != nullptr)
	{
		PerspectiveSetText->SetVisibility(ESlateVisibility::Visible);
	}

	if (MoveToPerspectiveBtn != nullptr)
	{
		MoveToPerspectiveBtn->SetIsEnabled(true);
	}
}

void UPerspectiveToolWidget::OnMoveToPerspectiveClicked()
{
	if (PerspectiveTool.IsValid())
	{
		PerspectiveTool->MoveToCorrectView();
	}
}

void UPerspectiveToolWidget::OnDoneBtnClicked()
{
	if (DoneOverlay != nullptr)
	{
		DoneOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (PerspectiveTool.IsValid())
	{
		PerspectiveTool->OnPerspectificationCompleted();
	}
}


void UPerspectiveToolWidget::NativeDestruct()
{
	if (DistanceSlider != nullptr)
	{
		DistanceSlider->OnValueChanged.RemoveDynamic(this, &UPerspectiveToolWidget::OnDistanceSliderValueChanged);
	}

	if (SetPerspectiveBtn != nullptr)
	{
		SetPerspectiveBtn->OnClicked.RemoveDynamic(this, &UPerspectiveToolWidget::OnSetPerspectiveClicked);
	}

	if (MoveToPerspectiveBtn != nullptr)
	{
		MoveToPerspectiveBtn->OnClicked.RemoveDynamic(this, &UPerspectiveToolWidget::OnMoveToPerspectiveClicked);
	}

	if (DoneBtn != nullptr)
	{
		DoneBtn->OnClicked.RemoveDynamic(this, &UPerspectiveToolWidget::OnDoneBtnClicked);
	}

	Super::NativeDestruct();
}
