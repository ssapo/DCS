#include "AN_UseItem.h"

#include "Components/EquipmentComponent.h"
#include "Components/InventoryComponent.h"

#include "DCSLib.h"

void UAN_UseItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UEquipmentComponent* CEquip = UDCSLib::GetComponent<UEquipmentComponent>(MeshComp->GetOwner());
	if (CEquip == nullptr)
	{
		return;
	}

	UInventoryComponent* CInven = UDCSLib::GetComponent<UInventoryComponent>(MeshComp->GetOwner());
	if (CInven == nullptr)
	{
		return;
	}

	if (const FStoredItem* ActiveItem = CEquip->GetActiveItem(Item, 0))
	{
		CInven->UseItem(ActiveItem->Id);
	}
}

FString UAN_UseItem::GetNotifyName_Implementation() const
{
	FString SEnum = UDCSLib::GetStringAsEnum(TEXT("EItem"), static_cast<int32>(Item));
	return FString::Format(TEXT("Use%s"), { SEnum });
}
