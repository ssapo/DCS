// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs.h"
#include "DCSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DCS_API ADCSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void LoadGame();
	void SaveGame();

	void UpdateEquipmentValues();
	void UpdateInventoryValues();
	void UpdateStatValues();

	const TArray<FEquipmentSlots>& GetEquipmentSlots() const { return EquipmentSlots; }
	EItem GetSelectedMainHandSlotType() const { return SelectedMainHandSlotType; }
	bool IsInCombat() const { return bIsInCombat; }

protected:
	virtual void BeginPlay() override;

	// start delcare event
public:
	DECLARE_EVENT(ADCSGameModeBase, FOnGameSaved);
	FOnGameSaved& OnGameSaved() { return GameSavedEvent; }

	DECLARE_EVENT(ADCSGameModeBase, FOnGameLoaded);
	FOnGameLoaded& OnGameLoaded() { return GameLoadedEvent; }

private:
	FOnGameSaved GameSavedEvent;
	FOnGameLoaded GameLoadedEvent;
	// end delcare event

private:
	TArray<FEquipmentSlots> EquipmentSlots;
	TArray<FStoredItem> Inventory;
	TMap<EStat, float> CurrentStatValues;
	EItem SelectedMainHandSlotType;
	bool bIsInCombat;
};
