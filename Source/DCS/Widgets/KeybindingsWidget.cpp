#include "KeybindingsWidget.h"
#include "Border.h"

void UKeybindingsWidget::ShowKeyBindings()
{
	Keybindings->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UKeybindingsWidget::HideKeyBindings()
{
	Keybindings->SetVisibility(ESlateVisibility::Collapsed);
}
