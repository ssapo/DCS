// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
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
	FStoredItem GetItemAtIndex(int32 Index) const;
	int32 FindIndexByClass(UClass* ItemClass) const;
	int32 FindIndexById(const FGuid& InID) const;

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
	UPROPERTY(EditInstanceOnly)
		TArray<FStoredItem> Inventory;
};
