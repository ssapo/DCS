
#pragma once

#include "CoreMinimal.h"
#include "Widgets/DCSWidget.h"
#include "Structs.h"
#include "WeakObjectPtrTemplates.h"
#include "InventoryItemWidget.generated.h"

class USizeBox;
class UImage;
class UInventoryComponent;
class UEquipmentComponent;
class UTextBlock;
class UButton;
class UItemGridWidget;

UCLASS()
class DCS_API UInventoryItemWidget : public UDCSWidget
{
	GENERATED_BODY()

protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetFocusedImage(bool InValue);
	void UpdateWidget(const FStoredItem& InItem);
	void UpdateAmountText();
	void UpdateIsEquippedImage();

private:
	void OnInItemSlotChanged(const FStoredItem& OldItem, const FStoredItem& NewItem, EItem InType, int32 InSlotIndex, int32 InActiveIndex);
	void OnActiveItemChanged(const FStoredItem& OldItem, const FStoredItem& NewItem, EItem InType, int32 InSlotIndex, int32 InActiveIndex);

	UFUNCTION()
		void OnClickedSlotButton();

	UFUNCTION()
		void OnHoveredSlotButton();

	UFUNCTION()
		void OnUnHoveredSlotButton();

private:
	TWeakObjectPtr<UInventoryComponent> WP_InventoryComponent;

	TWeakObjectPtr<UEquipmentComponent> WP_EquipmentComponent;

	UPROPERTY(meta = (BindWidget))
		UButton* SlotButton;

	UPROPERTY(meta = (BindWidget))
		UImage* FocusedImage;

	UPROPERTY(meta = (BindWidget))
		UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
		UImage* EquippedImage;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* AmountText;

	UPROPERTY(meta = (BindWidget))
		USizeBox* SlotSizeBox;

	UPROPERTY(EditAnywhere)
		UItemGridWidget* ItemsGridWidget;

	UPROPERTY(EditAnywhere)
		FLinearColor ActiveColor;

	UPROPERTY(EditAnywhere)
		FLinearColor InActiveColor;

	UPROPERTY(EditAnywhere)
		FVector2D SlotSize;

	FStoredItem Item;
};
