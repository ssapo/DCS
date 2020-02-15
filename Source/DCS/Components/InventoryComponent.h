// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	void UseItem(FGuid Id);

	FORCEINLINE TArray<FStoredItem> GetInventory() const { return Inventory; }

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
	TArray<FStoredItem> Inventory;
};
