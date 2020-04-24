#pragma once

#include "CoreMinimal.h"
#include "Widgets/DCSWidget.h"
#include "AIStatBarsWidget.generated.h"

class UStatBarWidget;

UCLASS()
class DCS_API UAIStatBarsWidget : public UDCSWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
		UStatBarWidget* Stamina;

	UPROPERTY(meta = (BindWidget))
		UStatBarWidget* Health;
};
