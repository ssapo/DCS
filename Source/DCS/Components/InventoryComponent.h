// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "SubclassOf.h"
#include "InventoryComponent.generated.h"

class UItemBase;
class UClass;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	void UseItem(const FGuid& Id);

	int32 FindIndexByClass(const TSubclassOf<UItemBase>& ItemClass) const;
	int32 FindIndexById(const FGuid& InID) const;
	FStoredItem FindInvenItemByEqItem(const FStoredItem& Item) const;
	FStoredItem GetItemAtIndex(int32 Index) const;

	const TArray<FStoredItem>& GetInventory() const { return Inventory; }

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void ClearInventory();
	void OnGameLoaded();
	void RemoveItemAtIndex(int32 Index, int32 Amount);

	bool IsSlotNotEmpty(int32 Index) const;
	bool IsSlotEmpty(int32 Index) const;

	// start Declare Events.
public:
	DECLARE_EVENT_OneParam(UInventoryComponent, FOnItemRemoved, const FStoredItem&);
	FOnItemRemoved& OnItemRemoved() { return ItemRemovedEvent; }

	DECLARE_EVENT_OneParam(UInventoryComponent, FOnItemAdded, const FStoredItem&);
	FOnItemAdded& OnItemAdded() { return ItemAddedEvent; }

private:
	FOnItemRemoved ItemRemovedEvent;
	FOnItemAdded ItemAddedEvent;
	// end Declare Events.

private:
	UPROPERTY(EditAnywhere)
		TArray<FStoredItem> Inventory;
};
