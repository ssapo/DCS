#include "ItemGridWidget.h"
#include "InventoryItemWidget.h"
#include "DCSLib.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"

void UItemGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UInventoryComponent* Raw = UDCSLib::GetComponent<UInventoryComponent>(this))
	{
		WP_InventoryComponent = Raw;
	}

	if (UEquipmentComponent* Raw = UDCSLib::GetComponent<UEquipmentComponent>(this))
	{
		WP_EquipmentComponent = Raw;
	}
}

void UItemGridWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UItemGridWidget::InventoryItemClicked(UInventoryItemWidget* InItemWidget)
{
	if (OnInventoryItemClicked.IsBound())
	{
		OnInventoryItemClicked.Broadcast(InItemWidget);
	}
}