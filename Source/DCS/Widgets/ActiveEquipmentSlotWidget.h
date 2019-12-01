#pragma once

#include "CoreMinimal.h"
#include "Widgets/DCSWidget.h"
#include "Structs.h"
#include "NoExportTypes.h"
#include "ActiveEquipmentSlotWidget.generated.h"

class UWidgetAnimation;
class UImage;
class UTextBlock;
class UBorder;
class USizeBox;
class UEquipmentComponent;
class UInventoryComponent;

UCLASS()
class DCS_API UActiveEquipmentSlotWidget : public UDCSWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidgetAnim))
		UWidgetAnimation* BlinkBorder;

protected:
	void NativeConstruct() override;
	void NativePreConstruct() override;
	void NativeDestruct() override;

	void UpdateWidget(const FStoredItem& InItem);
	void UpdateAmountText();
	void UpdateItemImage();
	void SetIsHidden(bool IsHidden);

private:
	TWeakObjectPtr<UInventoryComponent> WP_InventoryComponent;

	TWeakObjectPtr<UEquipmentComponent> WP_EquipmentComponent;

	UPROPERTY(meta = (BindWidget))
		UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
		UImage* IsHiddenImage;

	UPROPERTY(meta = (BindWidget))
		UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* AmountText;

	UPROPERTY(meta = (BindWidget))
		UBorder* BindBorder;

	UPROPERTY(meta = (BindWidget))
		USizeBox* SlotSizeBox;

	UPROPERTY(EditAnywhere)
		UTexture2D* BackgroundTexture;

	UPROPERTY(EditAnywhere)
		FVector2D SlotSize = FVector2D(128.0f, 128.0f);

	UPROPERTY(EditAnywhere)
		EItem ItemType;

	FStoredItem Item;
};
