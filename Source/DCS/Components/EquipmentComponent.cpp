
#include "EquipmentComponent.h"
#include "InventoryComponent.h"

#include "Interfaces/ItemCanBlock.h"
#include "Interfaces/ItemCanBeTwoHanded.h"

#include "Items/ObjectItems/ItemBase.h"

#include "DCSLib.h"

UEquipmentComponent::UEquipmentComponent()
{

}

void UEquipmentComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UEquipmentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UEquipmentComponent::Initialize()
{
	WP_Inventory = UDCSLib::GetComponent<UInventoryComponent>(GetOwner());
	if (WP_Inventory.IsValid() == false)
	{
		WP_Inventory->OnItemAdded().AddUObject(this, &UEquipmentComponent::OnItemModified);
		WP_Inventory->OnItemRemoved().AddUObject(this, &UEquipmentComponent::OnItemModified);
	}
}

int32 UEquipmentComponent::GetEquipmentSlotsIndex(EItem InType) const
{
	int32 SlotsNum = EquipmentSlots.Num();
	for (int32 I = 0; I < SlotsNum; ++I)
	{
		if (InType == EquipmentSlots[I].Type)
			return I;
	}

	return -1;
}

bool UEquipmentComponent::IsSlotIndexValid(EItem InType, int32 Index) const
{
	int32 GetIndex = GetEquipmentSlotsIndex(InType);
	if (EquipmentSlots.IsValidIndex(GetIndex))
	{
		return EquipmentSlots[GetIndex].Slots.IsValidIndex(Index);
	}

	return false;
}

bool UEquipmentComponent::IsItemIndexValid(EItem InType, int32 Index, int32 ItemIndex) const
{
	int32 GetIndex = GetEquipmentSlotsIndex(InType);
	if (EquipmentSlots.IsValidIndex(GetIndex))
	{
		const TArray<FEquipmentSlot>& Slots = EquipmentSlots[GetIndex].Slots;
		if (Slots.IsValidIndex(Index))
		{
			return Slots[Index].Items.IsValidIndex(ItemIndex);
		}
	}

	return false;
}

EItem UEquipmentComponent::GetItemType(const FStoredItem& InItem) const
{
	UItemBase* CDO = Cast<UItemBase>(InItem.ItemClass->ClassDefaultObject);
	return CDO->GetItemRef().Type;
}

TTuple<EItem, int32, int32> UEquipmentComponent::FindItem(const FStoredItem& InItem)
{
	EItem ItemType = GetItemType(InItem);

	return TTuple<EItem, int32, int32>();
}

void UEquipmentComponent::ToggleCombat()
{
	SetCombat(!bIsInCombat);
}

void UEquipmentComponent::SetCombat(bool InValue)
{
	if (bIsInCombat == InValue)
	{
		return;
	}

	bIsInCombat = InValue;

	CombatChangedEnvet.Broadcast(bIsInCombat);
}

void UEquipmentComponent::UpdateItemInSlot(EItem Type, int32 SlotIndex, int32 ItemIndex, const FStoredItem& InItem, EHandleSameItemMethod Method)
{

}

void UEquipmentComponent::OnItemModified(const FStoredItem& InItem)
{
	const TTuple<EItem, int32, int32>& Info = FindItem(InItem);
	EItem Type = Info.Get<0>();
	int32 SlotIndex = Info.Get<1>();
	int32 ItemIndex = Info.Get<2>();

	if (SlotIndex >= 0)
	{
		UpdateItemInSlot(Type, SlotIndex, ItemIndex, InItem, EHandleSameItemMethod::Update);
	}
}

bool UEquipmentComponent::IsSlotHidden(EItem InType, int32 Index) const
{
	if (IsSlotIndexValid(InType, Index))
	{
		return EquipmentSlots[GetEquipmentSlotsIndex(InType)].Slots[Index].IsHidden;
	}

	return false;
}

bool UEquipmentComponent::IsEquippedItem(const FGuid& InItemID) const
{
	return EquippedItems.Find(InItemID) >= 0;
}

bool UEquipmentComponent::IsActiveItem(const FGuid& InItemID) const
{
	return ActiveItems.Find(InItemID) >= 0;
}

bool UEquipmentComponent::IsShieldEquipped() const
{
	if (IsSlotHidden(EItem::Shield, 0))
	{
		return false;
	}

	int32 ItemIndex = GetActiveItemIndex(EItem::Shield, 0);
	const FStoredItem* StoredItem = GetItemInSlot(EItem::Shield, 0, ItemIndex);
	if (StoredItem == nullptr)
	{
		return false;
	}

	bool CanBlock = StoredItem->ItemClass->ImplementsInterface(UItemCanBlock::StaticClass());
	return CanBlock;
}

bool UEquipmentComponent::IsTwoHandedWeaponEquipped() const
{
	const FStoredItem* Weapon = GetWeapon();
	bool IsTwoHanded = Weapon->ItemClass->ImplementsInterface(UItemCanBeTwoHanded::StaticClass());

	return IsTwoHanded;
}

const FStoredItem* UEquipmentComponent::GetActiveItem(EItem InType, int32 Index) const
{
	int32 Result = GetEquipmentSlotsIndex(InType);
	if (EquipmentSlots.IsValidIndex(Result))
	{
		auto FSlots = EquipmentSlots[Result];

		auto Slot = FSlots.Slots[Index];
		return &Slot.Items[Slot.ActiveItemIndex];
	}

	return nullptr;
}

const FStoredItem* UEquipmentComponent::GetItemInSlot(EItem InType, int32 SlotIndex, int32 ItemIndex) const
{
	if (IsItemIndexValid(InType, SlotIndex, ItemIndex))
	{
		int32 EqSlotIndex = GetEquipmentSlotsIndex(InType);
		const FEquipmentSlots& EqSlots = EquipmentSlots[EqSlotIndex];
		return &EqSlots.Slots[SlotIndex].Items[ItemIndex];
	}

	return nullptr;
}

const FStoredItem* UEquipmentComponent::GetWeapon() const
{
	EItem Type = SelectedMainHandType;
	return GetItemInSlot(Type, 0, GetActiveItemIndex(Type, 0));
}

int32 UEquipmentComponent::GetActiveItemIndex(EItem InType, int32 InIndex) const
{
	int32 Index = GetEquipmentSlotsIndex(InType);
	if (EquipmentSlots.IsValidIndex(Index) == false)
	{
		return -1;
	}

	const TArray<FEquipmentSlot>& Slots = EquipmentSlots[Index].Slots;
	if (Slots.IsValidIndex(InIndex) == false)
	{
		return -1;
	}

	return Slots[InIndex].ActiveItemIndex;
}

ADisplayedItem* UEquipmentComponent::GetDisplayedItem(EItem InType, int32 Index) const
{
	if (DisplayedItems.Contains(InType) == false)
	{
		return nullptr;
	}

	FDisplayedItems FItem = DisplayedItems[InType];
	if (FItem.DisplayedItems.IsValidIndex(Index) == false)
	{
		return nullptr;
	}

	return FItem.DisplayedItems[Index];
}
