#include "Structs.h"
#include "Items/ObjectItems/ItemBase.h"

FCollCompHitActors::FCollCompHitActors() 
	: Component(nullptr)
	, HitActors()
{

}

FCollisionComponent::FCollisionComponent()
	: Component(nullptr)
	, Sockets()
{

}

FDisplayedItems::FDisplayedItems() 
	: DisplayedItemActors()
{

}

FDissolvedItems::FDissolvedItems() 
	: Component(nullptr)
	, Value(0)
	, Materials()
	, DissolveMaterials()
	, Reverse(false)
	, IsRunning(false)
{

}

FEffect::FEffect() 
	: Type(EEffect::None)
	, Duration(0)
	, Applier(nullptr)
{

}

FEquipmentSlot::FEquipmentSlot() 
	: Items()
	, ActiveItemIndex(0)
	, IsHidden(false)
{

}

FEquipmentSlots::FEquipmentSlots() 
	: Type(EItem::None)
	, Slots()
{

}

FHitData::FHitData() 
	: Damage(0)
	, DamageCauser(nullptr)
	, HitFromDirection(FVector::ZeroVector)
	, CanBeParried(false)
	, CanBeBlocked(true)
	, CanReceiveImpact(false)
{

}

FItem::FItem() 
	: Name("None")
	, Description(FText::FromString(TEXT("Item Description")))
	, Type(EItem::None)
	, IsStackable(false)
	, IsDroppable(true)
	, IsConsumable(false)
	, Image(nullptr)
{

}

FModifier::FModifier() 
	: Type(EStat::None)
	, Value(0)
{

}

FStat::FStat() 
	: FStat(EStat::None, 0.0f, 0.0f)
{

}

FStat::FStat(EStat InStat, float InBase, float InModifiersValue) 
	: Type(InStat)
	, BaseValue(InBase)
	, ModifiersValue(InModifiersValue)
{

}

FDemoRoomInfo::FDemoRoomInfo() 
	: RoomName()
	, RoomType(EDemoRoom::Standard)
	, OverrideSize(0)
{

}
