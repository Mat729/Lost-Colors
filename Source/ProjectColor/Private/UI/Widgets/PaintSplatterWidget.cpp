#include "UI/Widgets/PaintSplatterWidget.h"

void UPaintSplatterWidget::SetPaintSplatter()
{
	const float Scale = FMath::RandRange(MinScale, MaxScale);
	SetRenderScale(Scale * FVector2d::One());

	const float Angle = FMath::RandRange(0, 360);
	SetRenderTransformAngle(Angle);

	if (Colors.Num() > 0)
	{
		const FLinearColor Color = Colors[FMath::RandRange(0, Colors.Num() - 1)];
		SetColorAndOpacity(Color);
	}
}
