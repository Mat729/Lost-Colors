#include "UI/Menu/GamePageBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "UI/Widgets/PaintSplatterWidget.h"
#include "Kismet/GameplayStatics.h"

void UGamePageBase::SpawnPaintSplatter()
{
	if (!PaintSplatterWidgetClass) { return ; }

	UUserWidget* PaintSplatterWidget = UWidgetBlueprintLibrary::Create(this, PaintSplatterWidgetClass, nullptr);
	if (!PaintSplatterWidget) { return ; }
	
	UObject* Context = GetWorld();
	const FGeometry Geometry = UWidgetLayoutLibrary::GetViewportWidgetGeometry(Context);
	const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(Context);
	FVector2D PixelPos, ViewportPos;
	USlateBlueprintLibrary::LocalToViewport(Context, Geometry, MousePos, PixelPos, ViewportPos);

	PaintSplatterWidget->SetPositionInViewport(ViewportPos, false);

	if (UPaintSplatterWidget* PaintSplatter = Cast<UPaintSplatterWidget>(PaintSplatterWidget))
	{
		PaintSplatter->SetPaintSplatter();
	}

	UGameplayStatics::PlaySound2D(Context, PaintSplatterSound);
	
	PaintSplatterWidget->AddToViewport(1);
}
