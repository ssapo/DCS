// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveEquipmentSlotWidget.h"
#include "DCSLib.h"
#include "WidgetAnimation.h"
#include "Image.h"
#include "Border.h"
#include "TextBlock.h"
#include "SizeBox.h"
#include "Engine/Texture2D.h"
#include "Items/ObjectItems/ItemBase.h"
#include "WeakObjectPtrTemplates.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"
#include "Structs.h"

void UActiveEquipmentSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SlotSizeBox->SetWidthOverride(SlotSize.X);
	SlotSizeBox->SetHeightOverride(SlotSize.Y);
	BackgroundImage->SetBrushFromTexture(BackgroundTexture);
}

void UActiveEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto Pawn = GetOwningPlayerPawn();
	if (auto Raw = UDCSLib::GetComponent<UInventoryComponent>(Pawn))
	{
		WP_InventoryComponent = Raw;
	}

	if (auto Raw = UDCSLib::GetComponent<UEquipmentComponent>(Pawn))
	{
		WP_EquipmentComponent = Raw;
	}

	if (WP_EquipmentComponent.IsValid())
	{
		if (auto ActiveItem = WP_EquipmentComponent->GetActiveItem(ItemType, 0))
		{
			UpdateWidget(*ActiveItem);
		}

		SetIsHidden(WP_EquipmentComponent->IsSlotHidden(ItemType, 0));
	}
}

void UActiveEquipmentSlotWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UActiveEquipmentSlotWidget::UpdateWidget(const FStoredItem& InItem)
{
	Item = InItem;

	UpdateItemImage();

	UpdateAmountText();

	PlayAnimation(BlinkBorder);
}

void UActiveEquipmentSlotWidget::UpdateAmountText()
{
	bool Result = UDCSLib::IsValidClass(Item.ItemClass);
	if (Result)
	{
		UItemBase& DefaultItem = *Item.ItemClass.GetDefaultObject();
		
		if (DefaultItem.GetItemRef().IsStackable && Item.Amount > 1)
		{
			AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
			AmountText->SetText(FText::AsNumber(Item.Amount));
		}
		else
		{
			AmountText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		AmountText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UActiveEquipmentSlotWidget::UpdateItemImage()
{
	bool Result = UDCSLib::IsValidClass(Item.ItemClass);
	if (Result)
	{
		ItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		UItemBase& DefaultItem = *Item.ItemClass.GetDefaultObject();
		ItemImage->SetBrushFromTexture(DefaultItem.GetItemRef().Image);
	}
	else
	{
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		ItemImage->SetBrushFromTexture(nullptr);
	}
}

void UActiveEquipmentSlotWidget::SetIsHidden(bool IsHidden)
{
	if (IsHidden)
	{
		IsHiddenImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		IsHiddenImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	BindBorder->SetIsEnabled(!IsHidden);
}
