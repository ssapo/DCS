// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"

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

bool UEquipmentComponent::IsSlotHidden(EItem InType, int32 Index)
{
	if (IsSlotIndexValid(InType, Index))
	{
		return EquipmentSlots[GetEquipmentSlotsIndex(InType)].Slots[Index].IsHidden;
	}

	return false;
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
