#pragma once

#include "CoreMinimal.h"
#include "Widgets/DCSWidget.h"
#include "CategoryButtonWidget.generated.h"

class UButton;
class USizeBox;
class UImage;

UCLASS()
class DCS_API UCategoryButtonWidget : public UDCSWidget
{
	GENERATED_BODY()
	
protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
		USizeBox* SlotSizeBox;

	UPROPERTY(meta = (BindWidget))
		UButton* SlotButton;
	
	UPROPERTY(EditAnywhere)
		UImage* CategoryImage;

	UPROPERTY(EditAnywhere)
		UTexture2D* CategoryTexture;

	UPROPERTY(EditAnywhere)
		float SlotSize;
};
