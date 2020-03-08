#include "ItemMeleeWeapon.h"
#include "GameFramework/Actor.h"
#include "Items/DisplayedItems/DisplayedItem.h"

bool UItemMeleeWeapon::IsTwoHanded() const
{
	return bTwoHanded;
}

float UItemMeleeWeapon::GetBlockValue() const
{
	return BlockValue;
}

const TSubclassOf<ADisplayedItem>& UItemMeleeWeapon::GetDisplayedItem() const
{
	return DisplayedItemClass;
}

const TArray<FModifier>& UItemMeleeWeapon::GetModifiers() const
{
	return Modifiers;
}
