#include "ItemBase.h"
#include "GameFramework/Actor.h"

const FItem& UItemBase::GetItemRef() const
{
	return Item;
}
