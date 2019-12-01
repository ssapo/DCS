#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "EquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemInSlotChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActiveItemChanged);

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
	bool IsSlotHidden(EItem InType, int32 Index) const;
	bool IsEquippedItem(const FGuid& InItemID) const;
	bool IsActiveItem(const FGuid& InItemID) const;
private:
	int32 GetEquipmentSlotsIndex(EItem InType) const;
	bool IsSlotIndexValid(EItem InType, int32 Index) const;

public:
	FOnItemInSlotChanged OnItemInSlotChanged;
	FOnActiveItemChanged OnActiveItemChanged;

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
