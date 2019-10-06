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
	ADCSGameModeBase();

	void LoadGame();
	void SaveGame();

	void UpdateEquipmentValues();
	void UpdateInventoryValues();
	void UpdateStatValues();

protected:
	virtual void BeginPlay() override;

private:
	TArray<FEquiopmentSlots> EquipmentSlots;
	TArray<FStoredItem> Inventory;
	TMap<EStat, float> CurrentStatValues;
	EItem SelectedMainHandSlotType;
	bool bIsInCombat;
};
