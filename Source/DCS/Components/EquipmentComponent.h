// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "EquipmentComponent.generated.h"

class UInventoryComponent;
class AGameMode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();

	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FORCEINLINE TArray<FEquiopmentSlots> GetEquipmentSlots() const { return EquipmentSlots; }
	FORCEINLINE bool IsInCombat() const { return bIsInCombat; }
	FORCEINLINE EItem GetSelectedMainHandType() const { return SelectedMainHandType; }
	
	const FStoredItem* GetActiveItem(EItem InType, int32 Index) const;
	bool IsSlotHidden(EItem InType, int32 Index);

private:
	int32 GetEquipmentSlotsIndex(EItem InType) const;
	bool IsSlotIndexValid(EItem InType, int32 Index) const;

private:
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;

	TArray<FEquiopmentSlots> EquipmentSlots;
	TArray<EItem> MainHandTypes;
	TArray<FGuid> EquippedItems;
	TArray<FGuid> ActiveItems;

	TMap<EItem, FDisplayedItems> DisplayedItems;

	ECombat CombatType;
	EWeapon WepaonType;
	EItem SelectedMainHandType;

	bool bIsInCombat;
};
