#include "DI_Hand.h"
#include "Components/EquipmentComponent.h"

ADI_Hand::ADI_Hand()
{
}

void ADI_Hand::BeginPlay()
{
	Super::BeginPlay();

	if (WP_Equip.IsValid())
	{
		WP_Equip->OnSlotHiddenChanged().AddUObject(this, &ADI_Hand::OnSlotHiddenChanged);
	}
}

void ADI_Hand::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (WP_Equip.IsValid())
	{
		WP_Equip->OnSlotHiddenChanged().RemoveAll(this);
	}
}

void ADI_Hand::OnSlotHiddenChanged(EItem InSlotType, int32 InSlotIndex, const FStoredItem& ActiveItem, bool IsHidden)
{
	if (WP_Equip.IsValid() == false)
	{
		return;
	}

	if (WP_Equip->IsInCombat() == false)
	{
		return;
	}

	if (InSlotType != ItemType)
	{
		return;
	}

	if (InSlotIndex != SlotIndex)
	{
		return;
	}

	Attach();
}

FName ADI_Hand::GetAttachmentSocket() const
{
	if (WP_Equip.IsValid() == false)
	{
		return AttachmentSocket;
	}

	if (WP_Equip->IsSlotHidden(ItemType, SlotIndex))
	{
		return AttachmentSocket;
	}
	else
	{
		if (WP_Equip->IsInCombat())
		{
			return HandAttachmentSocket;
		}
		else
		{
			return AttachmentSocket;
		}
	}
}
