#include "InventoryItemWidget.h"
#include "SizeBox.h"
#include "Image.h"
#include "Components/EquipmentComponent.h"
#include "DCSLib.h"
#include "Items/ObjectItems/ItemBase.h"
#include "TextBlock.h"
#include "Button.h"
#include "ItemGridWidget.h"
#include "Structs.h"

void UInventoryItemWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SlotSizeBox->SetWidthOverride(SlotSize.X);
	SlotSizeBox->SetHeightOverride(SlotSize.Y);
	
}

void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetFocusedImage(false);

	check(WP_EquipmentComponent.IsValid());
	WP_EquipmentComponent->OnInSlotChanged().AddUObject(this, &UInventoryItemWidget::OnInItemSlotChanged);
	WP_EquipmentComponent->OnActiveItemChanged().AddUObject(this, &UInventoryItemWidget::OnActiveItemChanged);

	SlotButton->OnClicked.AddDynamic(this, &UInventoryItemWidget::OnClickedSlotButton);
	SlotButton->OnHovered.AddDynamic(this, &UInventoryItemWidget::OnHoveredSlotButton);
}

void UInventoryItemWidget::NativeDestruct()
{
	Super::NativeDestruct();

	check(WP_EquipmentComponent.IsValid());
	WP_EquipmentComponent->OnInSlotChanged().RemoveAll(this);
	WP_EquipmentComponent->OnActiveItemChanged().RemoveAll(this);

	SlotButton->OnClicked.RemoveAll(this);
	SlotButton->OnHovered.RemoveAll(this);
}

void UInventoryItemWidget::SetFocusedImage(bool InValue)
{
	if (InValue)
	{
		FocusedImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		FocusedImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryItemWidget::UpdateIsEquippedImage()
{
	if (WP_EquipmentComponent->IsEquippedItem(Item.Id))
	{
		EquippedImage->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		EquippedImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (WP_EquipmentComponent->IsActiveItem(Item.Id))
	{
		EquippedImage->SetColorAndOpacity(ActiveColor);
	}
	else
	{
		EquippedImage->SetColorAndOpacity(InActiveColor);
	}
}

void UInventoryItemWidget::OnInItemSlotChanged(const FStoredItem& OldItem, const FStoredItem& NewItem, EItem InType, int32 InSlotIndex, int32 InActiveIndex)
{

}

void UInventoryItemWidget::OnActiveItemChanged(const FStoredItem& OldItem, const FStoredItem& NewItem, EItem InType, int32 InSlotIndex, int32 InActiveIndex)
{

}

void UInventoryItemWidget::OnClickedSlotButton()
{
	ItemsGridWidget->InventoryItemClicked(this);
}

void UInventoryItemWidget::OnHoveredSlotButton()
{
	SetFocusedImage(true);
}

void UInventoryItemWidget::OnUnHoveredSlotButton()
{
	SetFocusedImage(false);
}

void UInventoryItemWidget::UpdateWidget(const FStoredItem& InItem)
{
	Item = InItem;

	bool IsValid = UDCSLib::IsValidClass(Item.ItemClass);
	bool NotZero = Item.Amount > 0;

	if (IsValid && NotZero)
	{
		SetVisibility(ESlateVisibility::Visible);

		UItemBase* Default = Item.ItemClass.GetDefaultObject();
		ItemImage->SetBrushFromTexture(Default->GetItemRef().Image);

		UpdateAmountText();
		UpdateIsEquippedImage();
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventoryItemWidget::UpdateAmountText()
{
	if (Item.Amount > 1)
	{
		AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
		AmountText->SetText(FText::AsNumber(Item.Amount));
	}
	else
	{
		AmountText->SetVisibility(ESlateVisibility::Hidden);
	}
}
