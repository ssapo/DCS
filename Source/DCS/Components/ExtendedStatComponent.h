// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "Components/ActorComponent.h"
#include "DelegateCombinations.h"
#include "ExtendedStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, float, InValue, float, MaxValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UExtendedStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExtendedStatComponent(); 

	EStat GetStatType() const { return StatType; }
	float GetCurrentValue() const { return CurrentValue; }
	float GetMaxValue() const { return TopValue; }

public:
	FOnValueChanged OnValueChanged;

private:
	EStat StatType;

	float RegenerationTickInterval;
	float CurrentValue;
	float TopValue;
	float ModifierValue;
	float RegenValue;
	float ReenableRegenTime;
	float InitialRegenValue;

	FTimerHandle RegenHandle;
	bool bDoesRegenerates;
};
