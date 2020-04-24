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
	void Finalize();

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
	bool IsActiveItemIndex(EItem InType, int32 SlotIndex, int32 ItemIndex) const;
	bool IsShieldEquipped() const;
	bool IsTwoHandedWeaponEquipped() const;
	bool CanBlock() const;

	void ToggleCombat();

private:
	void OnGameLoaded();
	void OnItemModified(const FStoredItem& InItem);
	void ActiveItemChanged(const FStoredItem& Old , const FStoredItem& New, EItem InType, int32 SlotIndex, int32 ItemIndex);
	void SetCombat(bool InValue);
	void SetSlotActiveIndex(EItem Type, int32 SlotIndex, int32 NewActiveIndex);
	void SetSlotHidden(EItem Type, int32 SlotIndex, bool bInHidden);
	void SetItemInSlot(EItem Type, int32 SlotIndex, int32 ItemIndex, const FStoredItem& InItem);
	void UpdateItemInSlot(EItem Type, int32 SlotIndex, int32 ItemIndex, const FStoredItem& InItem, EHandleSameItemMethod Method);
	void UpdateDisplayedItem(EItem InType, int32 SlotIndex);
	void AttachDisplayedItem(EItem InType, int32 SlotIndex);
	void UpdateCombatType();
	void BuildEquipment(const TArray<FEquipmentSlots>& EquipmentSlots);
	TTuple<EItem, int32, int32> FindItem(const FStoredItem& InItem);
	int32 GetEquipmentSlotsIndex(EItem InType) const;
	bool IsSlotIndexValid(EItem InType, int32 Index) const;
	bool IsItemIndexValid(EItem InType, int32 Index, int32 ItemIndex) const;
	bool IsValidItem(const FStoredItem& Item) const;
	bool IsValidItem(const FStoredItem* Item) const;
	bool IsItemTwoHanded(const FStoredItem& NewItem) const;
	bool IsRangeTypeCurrentMainHand() const;
	bool IsRangeTypeCurrentCombat() const;
	EItem GetItemType(const FStoredItem& InItem) const;

	// start declare events.
public:
	DECLARE_EVENT_FiveParams(UEquipmentComponent, FOnItemInSlotChanged, const FStoredItem&, const FStoredItem&, EItem, int32, int32);
	FOnItemInSlotChanged& OnInSlotChanged() { return ItemInSlotChangedEvent; }
	
	DECLARE_EVENT_FiveParams(UEquipmentComponent, FOnActiveItemChanged, const FStoredItem&, const FStoredItem&, EItem, int32, int32);
	FOnActiveItemChanged& OnActiveItemChanged() { return ActiveItemChangedEvent; }

	DECLARE_EVENT_FourParams(UEquipmentComponent, FOnSlotHiddenChanged, EItem, int32, const FStoredItem&, bool);
	FOnSlotHiddenChanged& OnSlotHiddenChanged() { return SlotHiddenChangedEvent; }

	DECLARE_EVENT_OneParam(UEquipmentComponent, FOnCombatChanged, bool);
	FOnCombatChanged& OnCombatChanged() { return CombatStatusChangedEnvet; }

	DECLARE_EVENT_OneParam(UEquipmentComponent, FOnWeaponTypeChanged, EWeapon);
	FOnWeaponTypeChanged& OnWeaponTypeChanged() { return WeaponTypeChangedEvent; }

	DECLARE_EVENT_OneParam(UEquipmentComponent, FOnMainHandTypeChanged, EItem);
	FOnMainHandTypeChanged& OnMainHandTypeChanged() { return MainHandTypeChangedEvent; }

	DECLARE_EVENT_OneParam(UEquipmentComponent, FOnCombatTypeChanged, ECombat);
	FOnCombatTypeChanged& OnCombatTypeChanged() { return CombatTypeChangedEvent; }

private:
	FOnWeaponTypeChanged WeaponTypeChangedEvent;
	FOnMainHandTypeChanged MainHandTypeChangedEvent;
	FOnSlotHiddenChanged SlotHiddenChangedEvent;
	FOnItemInSlotChanged ItemInSlotChangedEvent;
	FOnActiveItemChanged ActiveItemChangedEvent;
	FOnCombatChanged CombatStatusChangedEnvet;
	FOnCombatTypeChanged CombatTypeChangedEvent;
	// end declare events.

private:
	TWeakObjectPtr<UInventoryComponent> WP_Inventory;

	UPROPERTY(EditAnywhere)
		TArray<FEquipmentSlots> EquipmentSlots;

	TArray<EItem> MainHandTypes;
	TArray<FGuid> EquippedItems;
	TArray<FGuid> ActiveItems;
	
	TMap<EItem, FDisplayedItems> DisplayedItems;

	ECombat CombatType;
	EWeapon WeaponType;
	EItem SelectedMainHandType;

	bool bIsInCombat;

};
