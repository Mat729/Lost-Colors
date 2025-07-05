#include "UI/Menu/IntroPage.h"

#include "Components/CanvasPanel.h"

void UIntroPage::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UIntroPage::WidgetFadeOut()
{
	SetRenderOpacity(0.f);
}
