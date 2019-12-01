#pragma once

#include "CoreMinimal.h"
#include "Widgets/DCSWidget.h"
#include "KeybindingsWidget.generated.h"

class UBorder;

UCLASS()
class DCS_API UKeybindingsWidget : public UDCSWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

public:
	void ShowKeyBindings();
	void HideKeyBindings();

private:
	UPROPERTY(meta = (BindWidget))
		UBorder* Keybindings;
};
