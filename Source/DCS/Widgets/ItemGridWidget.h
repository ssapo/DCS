#pragma once

#include "CoreMinimal.h"
#include "Widgets/DCSWidget.h"
#include "DelegateCombinations.h"
#include "WeakObjectPtrTemplates.h"
#include "ItemGridWidget.generated.h"

class UInventoryItemWidget;
class UEquipmentComponent;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemClicked, UInventoryItemWidget*, InItem);

UCLASS()
class DCS_API UItemGridWidget : public UDCSWidget
{
	GENERATED_BODY()
	
public:
	void InventoryItemClicked(UInventoryItemWidget* InItemWidget);

protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;

public:
	FOnInventoryItemClicked OnInventoryItemClicked;

private:
	TWeakObjectPtr<UInventoryComponent> WP_InventoryComponent;

	TWeakObjectPtr<UEquipmentComponent> WP_EquipmentComponent;

};
