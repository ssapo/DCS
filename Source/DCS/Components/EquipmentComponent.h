#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "DelegateCombinations.h"
#include "EquipmentComponent.generated.h"

class UInventoryComponent;
class AGameMode;
class ADisplayedItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();

	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Initialize();

	FORCEINLINE TArray<FEquipmentSlots> GetEquipmentSlots() const { return EquipmentSlots; }
	FORCEINLINE bool IsInCombat() const { return bIsInCombat; }
	FORCEINLINE EItem GetSelectedMainHandType() const { return SelectedMainHandType; }
	FORCEINLINE ECombat GetCombatType() const { return CombatType; }

	const FStoredItem* GetActiveItem(EItem InType, int32 Index) const;
	const FStoredItem* GetItemInSlot(EItem InType, int32 SlotIndex, int32 ItemIndex) const;
	const FStoredItem* GetWeapon() const;
	int32 GetActiveItemIndex(EItem InType, int32 Index) const;
	ADisplayedItem* GetDisplayedItem(EItem InType, int32 Index) const;
	
	bool IsSlotHidden(EItem InType, int32 Index) const;
	bool IsEquippedItem(const FGuid& InItemID) const;
	bool IsActiveItem(const FGuid& InItemID) const;

	bool IsShieldEquipped() const;
	bool IsTwoHandedWeaponEquipped() const;
	void ToggleCombat();

private:
	void OnItemModified(const FStoredItem& InItem);

	void SetCombat(bool InValue);
	void UpdateItemInSlot(EItem Type, int32 SlotIndex, int32 ItemIndex, const FStoredItem& InItem, EHandleSameItemMethod Method);
	void UpdateCombatType();
	TTuple<EItem, int32, int32> FindItem(const FStoredItem& InItem);
	int32 GetEquipmentSlotsIndex(EItem InType) const;
	bool IsSlotIndexValid(EItem InType, int32 Index) const;
	bool IsItemIndexValid(EItem InType, int32 Index, int32 ItemIndex) const;
	EItem GetItemType(const FStoredItem& InItem) const;

	// start Declare Events.
public:
	DECLARE_EVENT_FiveParams(UEquipmentComponent, FOnItemInSlotChanged, const FStoredItem&, const FStoredItem&, EItem, int32, int32);
	FOnItemInSlotChanged& OnInSlotChanged() { return ItemInSlotChangedEvent; }
	
	DECLARE_EVENT_FiveParams(UEquipmentComponent, FOnActiveItemChanged, const FStoredItem&, const FStoredItem&, EItem, int32, int32);
	FOnActiveItemChanged& OnActiveItemChanged() { return ActiveItemChangedEvent; }

	DECLARE_EVENT_OneParam(UEquipmentComponent, FOnCombatChanged, bool);
	FOnCombatChanged& OnCombatChanged() { return CombatChangedEnvet; }

	DECLARE_EVENT_OneParam(UEquipmentComponent, FOnWeaponTypeChanged, EWeapon);
	FOnWeaponTypeChanged& OnWeaponTypeChanged() { return WeaponTypeChangedEvent; }

	DECLARE_EVENT_OneParam(UEquipmentComponent, FOnMainHandTypeChanged, EItem);
	FOnMainHandTypeChanged& OnMainHandTypeChanged() { return MainHandTypeChangedEvent; }

	DECLARE_EVENT_OneParam(UEquipmentComponent, FOnCombatTypeChanged, ECombat);
	FOnCombatTypeChanged& OnCombatTypeChanged() { return CombatTypeChangedEvent; }

private:
	FOnWeaponTypeChanged WeaponTypeChangedEvent;
	FOnMainHandTypeChanged MainHandTypeChangedEvent;
	FOnItemInSlotChanged ItemInSlotChangedEvent;
	FOnActiveItemChanged ActiveItemChangedEvent;
	FOnCombatChanged CombatChangedEnvet;
	FOnCombatTypeChanged CombatTypeChangedEvent;
	// end Declare Events.

private:
	TWeakObjectPtr<UInventoryComponent> WP_Inventory;

	TArray<FEquipmentSlots> EquipmentSlots;
	TArray<EItem> MainHandTypes;
	TArray<FGuid> EquippedItems;
	TArray<FGuid> ActiveItems;

	TMap<EItem, FDisplayedItems> DisplayedItems;

	ECombat CombatType;
	EWeapon WepaonType;
	EItem SelectedMainHandType;

	bool bIsInCombat;

};
