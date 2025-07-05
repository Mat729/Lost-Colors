#include "UI/Menu/GameButtonBaseWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UGameButtonBaseWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetText(ButtonText);

	if (!Button) {return;}
	Button->OnReleased.AddDynamic(this, &UGameButtonBaseWidget::OnButtonClicked);

}

void UGameButtonBaseWidget::SetText(const FText& InText) const
{
	if (!TextBlock) {return;}
	TextBlock->SetText(InText);
}

void UGameButtonBaseWidget::OnButtonClicked()
{
	ButtonClickedDelegate.Broadcast();
	UE_LOG(LogTemp,Warning,TEXT("Clicked"));
}
