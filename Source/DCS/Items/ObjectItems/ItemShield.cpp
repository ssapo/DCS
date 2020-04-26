#include "ItemShield.h"
#include "GameFramework/Actor.h"

float UItemShield::GetBlockValue() const
{
	return BlockValue;
}

const TSubclassOf<ADisplayedItem>& UItemShield::GetDisplayedItem() const
{
	return DisplayedItemClass;
}
