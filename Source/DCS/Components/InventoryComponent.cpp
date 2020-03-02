#include "InventoryComponent.h"

#include "Items/ObjectItems/ItemBase.h"

#include "DCSLib.h"
#include "DCSGameModeBase.h"

UInventoryComponent::UInventoryComponent()
{
	// TODO: fill function
}

void UInventoryComponent::UseItem(const FGuid& InId)
{
	int32 Index = FindIndexById(InId);
	if (IsSlotNotEmpty(Index))
	{
		const FStoredItem& StoredItem = Inventory[Index];
		UItemBase* NewItem = NewObject<UItemBase>(GetOwner(), StoredItem.ItemClass);
		NewItem->UseItem(GetOwner());

		if (StoredItem.ItemClass->GetDefaultObject<UItemBase>()->GetItemRef().IsConsumable)
		{
			RemoveItemAtIndex(Index, 1);
		}
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ClearInventory();

	if (UDCSLib::OwnerIsPlayer(GetOwner()))
	{
		if (ADCSGameModeBase* GameMode = UDCSLib::GetGameMode<ADCSGameModeBase>(this))
		{
			GameMode->OnGameLoaded().AddUObject(this, &UInventoryComponent::OnGameLoaded);
		}
	}
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UDCSLib::OwnerIsPlayer(GetOwner()))
	{
		if (ADCSGameModeBase * GameMode = UDCSLib::GetGameMode<ADCSGameModeBase>(this))
		{
			GameMode->OnGameLoaded().RemoveAll(this);
		}
	}
}

void UInventoryComponent::ClearInventory()
{
	for (int32 I = 0; I < Inventory.Num(); ++I)
	{
		FStoredItem& Item = Inventory[I];
		if (IsSlotNotEmpty(I))
		{
			if (Item.ItemClass->GetDefaultObject<UItemBase>()->GetItemRef().IsStackable == false)
			{
				Item.Amount = 1;
			}
		}
		else
		{
			Inventory.RemoveAt(I);
			--I;
		}
	}
}

void UInventoryComponent::OnGameLoaded()
{
	if (ADCSGameModeBase* GameMode = UDCSLib::GetGameMode<ADCSGameModeBase>(this))
	{
		Inventory = GameMode->Inventory;

		ClearInventory();
	}
}

void UInventoryComponent::RemoveItemAtIndex(int32 Index, int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	if (IsSlotEmpty(Index))
	{
		return;
	}

	FStoredItem& StoredItem = Inventory[Index];
	StoredItem.Amount -= Amount;

	ClearInventory();

	ItemRemovedEvent.Broadcast(StoredItem);
}

bool UInventoryComponent::IsSlotNotEmpty(int32 Index) const
{
	if (Inventory.IsValidIndex(Index) == false)
	{
		return false;
	}

	return UDCSLib::IsItemValid(&Inventory[Index]);
}

bool UInventoryComponent::IsSlotEmpty(int32 Index) const
{
	return IsSlotNotEmpty(Index) == false;
}

FStoredItem UInventoryComponent::GetItemAtIndex(int32 Index) const
{
	FStoredItem ReturnValue;
	if (IsSlotNotEmpty(Index))
	{
		ReturnValue = Inventory[Index];
	}

	return ReturnValue;
}

int32 UInventoryComponent::FindIndexByClass(UClass* ItemClass) const
{
	for (int32 I = 0; I < Inventory.Num(); ++I)
	{
		const FStoredItem& Item = Inventory[I];

		if (Item.ItemClass == ItemClass && Item.Amount >= 1)
		{
			return I;
		}
	}

	return UDCSLib::INV_INDEX;
}

int32 UInventoryComponent::FindIndexById(const FGuid& InID) const
{
	for (int32 I = 0; I < Inventory.Num(); ++I)
	{
		const FStoredItem& Item = Inventory[I];

		if (Item.Id == InID && Item.Amount >= 1)
		{
			return I;
		}
	}

	return UDCSLib::INV_INDEX;
}
