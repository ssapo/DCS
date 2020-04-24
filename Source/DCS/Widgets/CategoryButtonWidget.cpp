// Fill out your copyright notice in the Description page of Project Settings.


#include "CategoryButtonWidget.h"
#include "SizeBox.h"
#include "DCSLib.h"
#include "Image.h"
#include "InventoryWidget.h"

void UCategoryButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SlotSizeBox->SetWidthOverride(SlotSize);
	SlotSizeBox->SetHeightOverride(SlotSize);
	
	if (CategoryImage)
	{
		CategoryImage->SetBrushFromTexture(CategoryTexture);
	}
}

void UCategoryButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UInventoryWidget*> FoundWidgets = UDCSLib::GetWidgets<UInventoryWidget>(this);
	if (FoundWidgets.IsValidIndex(0))
	{
		UInventoryWidget* Inventory = FoundWidgets[0];
		//Inventory->GetDisplayedType()
	}
}
