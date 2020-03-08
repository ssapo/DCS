
#include "EquipmentComponent.h"
#include "InventoryComponent.h"

#include "Interfaces/ItemCanBlock.h"
#include "Interfaces/ItemCanBeTwoHanded.h"

#include "Items/ObjectItems/ItemBase.h"
#include "Items/ObjectItems/ItemWeapon.h"
#include "Items/DisplayedItems/DisplayedItem.h"

#include "DCSLib.h"
#include "DCSGameModeBase.h"

UEquipmentComponent::UEquipmentComponent()
{
	MainHandTypes.Add(EItem::MeleeWeapon);
	MainHandTypes.Add(EItem::Spell);
	MainHandTypes.Add(EItem::RangeWeapon);

	SelectedMainHandType = EItem::MeleeWeapon;

	CombatType = ECombat::Unarmed;
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
	if (WP_Inventory.IsValid())
	{
		WP_Inventory->OnItemAdded().AddUObject(this, &UEquipmentComponent::OnItemModified);
		WP_Inventory->OnItemRemoved().AddUObject(this, &UEquipmentComponent::OnItemModified);
	}

	BuildEquipment(EquipmentSlots);

	APlayerController* PlayerController = UDCSLib::GetPlayerController(this, 0);
	check(PlayerController);

	if (GetOwner() != PlayerController)
	{
		return;
	}
	
	if (ADCSGameModeBase* GameMode = UDCSLib::GetGameMode<ADCSGameModeBase>(this))
	{
		GameMode->OnGameLoaded().AddUObject(this, &UEquipmentComponent::OnGameLoaded);
	}
}

void UEquipmentComponent::Finalize()
{
	WP_Inventory = UDCSLib::GetComponent<UInventoryComponent>(GetOwner());
	if (WP_Inventory.IsValid())
	{
		WP_Inventory->OnItemAdded().RemoveAll(this);
		WP_Inventory->OnItemRemoved().RemoveAll(this);
	}

	if (ADCSGameModeBase * GameMode = UDCSLib::GetGameMode<ADCSGameModeBase>(this))
	{
		GameMode->OnGameLoaded().RemoveAll(this);
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

	return UDCSLib::INV_INDEX;
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

bool UEquipmentComponent::IsItemValid(const FStoredItem& Item) const
{
	return UDCSLib::IsItemValid(&Item);
}

bool UEquipmentComponent::IsItemValid(const FStoredItem* Item) const
{
	return UDCSLib::IsItemValid(Item);
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

	CombatStatusChangedEnvet.Broadcast(bIsInCombat);
}

void UEquipmentComponent::SetSlotActiveIndex(EItem Type, int32 SlotIndex, int32 NewActiveIndex)
{
	if (IsSlotIndexValid(Type, SlotIndex) == false)
	{
		return;
	}

	int32 ActiveItemIndex = GetActiveItemIndex(Type, SlotIndex);
	if (ActiveItemIndex == UDCSLib::INV_INDEX)
	{
		return;
	}

	int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
	if (EqSlotsIndex == UDCSLib::INV_INDEX)
	{
		return;
	}

	if (ActiveItemIndex == NewActiveIndex)
	{
		return;
	}

	FEquipmentSlots& EqSlots = EquipmentSlots[EqSlotsIndex];
	FEquipmentSlot& EqSlot = EqSlots.Slots[SlotIndex];
	EqSlot.ActiveItemIndex = NewActiveIndex;

	const FStoredItem& OldItem = EqSlot.Items[ActiveItemIndex];
	const FStoredItem& NewItem = EqSlot.Items[NewActiveIndex];

	ActiveItemChangedEvent.Broadcast(OldItem, NewItem, Type, SlotIndex, NewActiveIndex);
}

void UEquipmentComponent::SetSlotHidden(EItem Type, int32 SlotIndex, bool bInHidden)
{
	if (IsSlotHidden(Type, SlotIndex) == bInHidden)
	{
		return;
	}

	int32 Index = GetEquipmentSlotsIndex(Type);
	FEquipmentSlots& EqSlots = EquipmentSlots[Index];
	FEquipmentSlot& EqSlot = EqSlots.Slots[SlotIndex];
	EqSlot.IsHidden = bInHidden;

	int32 ActiveItemIndex = GetActiveItemIndex(Type, SlotIndex);
	const FStoredItem* ActiveItem = GetItemInSlot(Type, SlotIndex, ActiveItemIndex);
	if (IsItemValid(ActiveItem) == false)
	{
		return;
	}

	SlotHiddenChangedEvent.Broadcast(Type, SlotIndex, *ActiveItem, bInHidden);
}

void UEquipmentComponent::SetItemInSlot(EItem Type, int32 SlotIndex, int32 ItemIndex, const FStoredItem& InItem)
{
	int32 Index = GetEquipmentSlotsIndex(Type);
	FEquipmentSlots& EqSlots = EquipmentSlots[Index];
	FEquipmentSlot& EqSlot = EqSlots.Slots[SlotIndex];
	EqSlot.Items.EmplaceAt(ItemIndex, InItem);
}

void UEquipmentComponent::UpdateItemInSlot(EItem Type, int32 SlotIndex, int32 ItemIndex, const FStoredItem& InItem, EHandleSameItemMethod Method)
{
	if (IsItemIndexValid(Type, SlotIndex, ItemIndex) == false)
	{
		return;
	}

	if (IsItemValid(InItem))
	{
		EItem ItemType = GetItemType(InItem);

		if (ItemType != Type)
		{
			return;
		}

		const FStoredItem* StItem = GetItemInSlot(Type, SlotIndex, ItemIndex);
		if (IsItemValid(StItem) == false)
		{
			return;
		}

		if (StItem->Id != InItem.Id)
		{
			// check if that item was already equipped in other slot index, if so, unequip it
			int32 EqSlotsIndex = GetEquipmentSlotsIndex(Type);
			const FEquipmentSlots& EqSlots = EquipmentSlots[EqSlotsIndex];

			for (int32 I = 0; I < EqSlots.Slots.Num(); ++I)
			{
				const FEquipmentSlot& EqSlot = EqSlots.Slots[I];
				for (int32 J = 0; J < EqSlot.Items.Num(); ++J)
				{
					const FStoredItem& Item = EqSlot.Items[J];

					if (InItem.Id == Item.Id)
					{
						SetItemInSlot(Type, I, J, Item);
						EquippedItems.Remove(Item.Id);

						ItemInSlotChangedEvent.Broadcast(Item, FStoredItem(), Type, I, J);

						if (IsActiveItemIndex(Type, I, J))
						{
							ActiveItemChangedEvent.Broadcast(Item, FStoredItem(), Type, I, J);
						}
					}
				}
			}
		}

		// Equip new item.
		const FStoredItem* OldItem = GetItemInSlot(Type, SlotIndex, ItemIndex);
		FStoredItem NewItem;
		if (Method != EHandleSameItemMethod::UnEquip)
		{
			NewItem = InItem;
		}

		SetItemInSlot(Type, SlotIndex, ItemIndex, InItem);

		EquippedItems.Remove(OldItem->Id);
		if (Method != EHandleSameItemMethod::UnEquip)
		{
			EquippedItems.Add(InItem.Id);
		}

		ItemInSlotChangedEvent.Broadcast(*OldItem, InItem, Type, SlotIndex, ItemIndex);

		if (IsActiveItemIndex(Type, SlotIndex, ItemIndex))
		{
			ActiveItemChangedEvent.Broadcast(*OldItem, InItem, Type, SlotIndex, ItemIndex);
		}
	}
	else
	{
		const FStoredItem* OldItem = GetItemInSlot(Type, SlotIndex, ItemIndex);
		FStoredItem NewItem;
		SetItemInSlot(Type, SlotIndex, ItemIndex, NewItem);

		EquippedItems.Remove(OldItem->Id);

		ItemInSlotChangedEvent.Broadcast(*OldItem, NewItem, Type, SlotIndex, ItemIndex);


		if (IsActiveItemIndex(Type, SlotIndex, ItemIndex))
		{
			ActiveItemChangedEvent.Broadcast(*OldItem, NewItem, Type, SlotIndex, ItemIndex);
		}
	}
}

void UEquipmentComponent::UpdateCombatType()
{
	ECombat PreviousCombatType = CombatType;
	EWeapon PreviousWeaponType = WeaponType;
	const FStoredItem* LastWeapon = GetWeapon();

	if (IsItemValid(LastWeapon))
	{
		switch (SelectedMainHandType)
		{
		case EItem::Spell: CombatType = ECombat::Magic; break;
		case EItem::MeleeWeapon: CombatType = ECombat::Melee; break;
		case EItem::RangeWeapon: CombatType = ECombat::Range; break;
		}
		
		if (UItemWeapon* WeaponCDO = Cast<UItemWeapon>(LastWeapon->ItemClass->ClassDefaultObject))
		{
			WeaponType = WeaponCDO->GetWeaponType();
		}
	}
	else
	{
		CombatType = ECombat::Unarmed;
		WeaponType = EWeapon::None;
	}

	if (CombatType != PreviousCombatType)
	{
		CombatTypeChangedEvent.Broadcast(CombatType);
	}

	if (WeaponType != PreviousWeaponType)
	{
		WeaponTypeChangedEvent.Broadcast(WeaponType);
	}
}

void UEquipmentComponent::BuildEquipment(const TArray<FEquipmentSlots>& InEquipmentSlots)
{
	TArray<FEquipmentSlots> _EquipmentSlots = InEquipmentSlots;
	int32 EqSlotsIndex = 0;
	int32 SlotIndex = 0;
	int32 ItemIndex = 0;
	int32 FoundItemIndex = 0;
	EItem ItemType = EItem::None;

	// Clear and rebuild displayed items array.
	TArray<FDisplayedItems> DisplayItemsArr;
	DisplayedItems.GenerateValueArray(DisplayItemsArr);
	for (const FDisplayedItems& E : DisplayItemsArr)
	{
		for (ADisplayedItem* F : E.DisplayedItemActors)
		{
			if (F) { F->Destroy(); }
		}
	}
	DisplayedItems.Empty();

	for (const FEquipmentSlots& E : _EquipmentSlots)
	{
		TArray<ADisplayedItem*> ItemActors;
		ItemActors.SetNum(E.Slots.Num());

		FDisplayedItems DisplayedItem;
		DisplayedItem.DisplayedItemActors = ItemActors;

		DisplayedItems.Add(E.Type, DisplayedItem);
	}

	// Unequip all items
	int32 EqsNum = InEquipmentSlots.Num();
	for (int32 I = 0; I < EqsNum; ++I)
	{
		const FEquipmentSlots& EqSlots = InEquipmentSlots[I];

		EqSlotsIndex = I;
		ItemType = EqSlots.Type;

		int32 SlotsNum = EqSlots.Slots.Num();
		for (int32 J = 0; J < SlotsNum; ++J)
		{
			const FEquipmentSlot& EqSlot = EqSlots.Slots[J];
			SlotIndex = J; 

			int32 ItemsNum = EqSlot.Items.Num();
			for (int32 K = 0; K < ItemsNum; ++K)
			{
				const FStoredItem& StoredItem = EqSlot.Items[K];
				ItemIndex = K;

				if (UDCSLib::IsItemValid(&StoredItem))
				{
					if (IsEquippedItem(StoredItem.Id))
					{
						UpdateItemInSlot(ItemType, SlotIndex, ItemIndex, FStoredItem(), EHandleSameItemMethod::UnEquip);
					}
					else
					{
						SetItemInSlot(ItemType, SlotIndex, ItemIndex, FStoredItem());
					}
				}
				else
				{
					SetItemInSlot(ItemType, SlotIndex, ItemIndex, FStoredItem());
				}
			}
		}
	}

	// Hide not selected main hand slot types (hide all and show only active to make sure OnSlotHiddenChanged event dispatcher will be triggered).
	for (const EItem& E : MainHandTypes)
	{
		int32 EquipmentSlotsIndex = GetEquipmentSlotsIndex(E);
		const FEquipmentSlots& EqSlots = InEquipmentSlots[EquipmentSlotsIndex];

		int32 EqSlotsNum = EqSlots.Slots.Num();
		for (int32 I = 0; I < EqSlotsNum; ++I)
		{
			bool IsHidden = (SelectedMainHandType != E);
			SetSlotHidden(EqSlots.Type, I, IsHidden);
		}
	}

	// Equip new items.
	int32 EqSlotsNum = _EquipmentSlots.Num();
	for (int32 I = 0; I < EqSlotsNum; ++I)
	{
		const FEquipmentSlots& EqSlots = _EquipmentSlots[I];

		EqSlotsIndex = I;
		ItemType = EqSlots.Type;

		int32 SlotsNum = EqSlots.Slots.Num();
		for (int32 J = 0; J < SlotsNum; ++J)
		{
			const FEquipmentSlot& EqSlot = EqSlots.Slots[J];
			SlotIndex = J;

			SetSlotActiveIndex(ItemType, SlotIndex, EqSlot.ActiveItemIndex);

			int32 ItemsNum = EqSlot.Items.Num();
			for (int32 K = 0; K < ItemsNum; ++K)
			{
				ItemIndex = K;
				const FStoredItem& StoredItem = EqSlot.Items[K];

				if (WP_Inventory.IsValid())
				{
					const FStoredItem& FoundItem = WP_Inventory->FindInvenItemByEqItem(StoredItem);
					if (IsItemValid(FoundItem))
					{
						UpdateItemInSlot(ItemType, SlotIndex, ItemIndex, FoundItem, EHandleSameItemMethod::Update);
					}
				}
				else
				{
					if (IsItemValid(StoredItem))
					{
						UpdateItemInSlot(ItemType, SlotIndex, ItemIndex, StoredItem, EHandleSameItemMethod::Update);
					}
				}
			}
		}
	}

	UpdateCombatType();
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

void UEquipmentComponent::OnGameLoaded()
{
	if (ADCSGameModeBase* GameMode = UDCSLib::GetGameMode<ADCSGameModeBase>(this))
	{
		SelectedMainHandType = GameMode->GetSelectedMainHandSlotType();
		BuildEquipment(GameMode->GetEquipmentSlots());
		SetCombat(GameMode->IsInCombat());
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

bool UEquipmentComponent::IsActiveItemIndex(EItem InType, int32 SlotIndex, int32 ItemIndex) const
{
	return GetActiveItemIndex(InType, SlotIndex) == ItemIndex;
}

bool UEquipmentComponent::IsShieldEquipped() const
{
	if (IsSlotHidden(EItem::Shield, 0))
	{
		return false;
	}

	int32 ItemIndex = GetActiveItemIndex(EItem::Shield, 0);
	const FStoredItem* StoredItem = GetItemInSlot(EItem::Shield, 0, ItemIndex);
	if (IsItemValid(StoredItem) == false)
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

bool UEquipmentComponent::CanBlock() const
{
	if (bIsInCombat)
	{
		bool bCanBlock = (ECombat::Melee == CombatType) || (ECombat::Unarmed == CombatType);
		return bCanBlock;
	}

	return false;
}

const FStoredItem* UEquipmentComponent::GetActiveItem(EItem InType, int32 Index) const
{
	int32 Result = GetEquipmentSlotsIndex(InType);
	if (EquipmentSlots.IsValidIndex(Result))
	{
		const FEquipmentSlots& FSlots = EquipmentSlots[Result];
		const FEquipmentSlot& Slot = FSlots.Slots[Index];
		return &(Slot.Items[Slot.ActiveItemIndex]);
	}

	return nullptr;
}

const FStoredItem* UEquipmentComponent::GetItemInSlot(EItem InType, int32 SlotIndex, int32 ItemIndex) const
{
	if (IsItemIndexValid(InType, SlotIndex, ItemIndex))
	{
		int32 EqSlotIndex = GetEquipmentSlotsIndex(InType);
		const FEquipmentSlots& EqSlots = EquipmentSlots[EqSlotIndex];
		return &(EqSlots.Slots[SlotIndex].Items[ItemIndex]);
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
		return UDCSLib::INV_INDEX;
	}

	const TArray<FEquipmentSlot>& Slots = EquipmentSlots[Index].Slots;
	if (Slots.IsValidIndex(InIndex) == false)
	{
		return UDCSLib::INV_INDEX;
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
	if (FItem.DisplayedItemActors.IsValidIndex(Index) == false)
	{
		return nullptr;
	}

	return FItem.DisplayedItemActors[Index];
}
