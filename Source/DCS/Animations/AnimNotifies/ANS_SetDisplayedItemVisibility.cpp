#include "ANS_SetDisplayedItemVisibility.h"

#include "DCSLib.h"
#include "Items/DisplayedItems/DisplayedItem.h"

#include "Components/EquipmentComponent.h"

UANS_SetDisplayedItemVisibility::UANS_SetDisplayedItemVisibility()
{
	Item = EItem::None;
	bHasToBeInCombat = false;
	bHideThenShow = true;
}

void UANS_SetDisplayedItemVisibility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UEquipmentComponent* CEquip = UDCSLib::GetComponent<UEquipmentComponent>(MeshComp->GetOwner());
	if (CEquip == nullptr)
	{
		return;
	}

	if (bHasToBeInCombat)
	{
		if (CEquip->IsInCombat() == false)
		{
			return;
		}
	}

	bool IsHidden = CEquip->IsSlotHidden(Item, 0);
	if (IsHidden)
	{
		return;
	}

	ADisplayedItem* DisplayedItem = CEquip->GetDisplayedItem(Item, 0);
	if (DisplayedItem == nullptr)
	{
		return;
	}

	DisplayedItem->SetActorHiddenInGame(bHideThenShow);
}

void UANS_SetDisplayedItemVisibility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	UEquipmentComponent* CEquip = UDCSLib::GetComponent<UEquipmentComponent>(MeshComp->GetOwner());
	if (CEquip == nullptr)
	{
		return;
	}

	if (bHasToBeInCombat)
	{
		if (CEquip->IsInCombat() == false)
		{
			return;
		}
	}

	bool IsHidden = CEquip->IsSlotHidden(Item, 0);
	if (IsHidden)
	{
		return;
	}

	ADisplayedItem* DisplayedItem = CEquip->GetDisplayedItem(Item, 0);
	if (DisplayedItem == nullptr)
	{
		return;
	}

	DisplayedItem->SetActorHiddenInGame(bHideThenShow == false);
}

FString UANS_SetDisplayedItemVisibility::GetNotifyName_Implementation() const
{
	FString HideThenShow = bHideThenShow ? TEXT("Hide then Show ") : TEXT("Show then Hide ");
	FString SItem = UDCSLib::GetStringAsEnum(TEXT("EItem"), static_cast<int32>(Item));
	FString HasToBeInCombat = bHasToBeInCombat ? TEXT("(Has to be in Combat)") : TEXT("");
	return HideThenShow + SItem + HasToBeInCombat;
}
