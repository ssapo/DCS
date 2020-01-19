// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "Components/ActorComponent.h"
#include "DelegateCombinations.h"
#include "ExtendedStatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UExtendedStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExtendedStatComponent(); 

	FORCEINLINE EStat GetStatType() const;
	FORCEINLINE float GetCurrentValue() const;
	FORCEINLINE float GetMaxValue() const;

	void ModifyStat(float InValue, bool InterruptRegeneration);

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void InitStatManager();

	void OnRegenTick();
	void OnStartRegenerating();
	void OnModifierAdded(EStat InType, float Value);
	void OnModifierRemoved(EStat InType, float Value);
	void OnBaseValueChanged(EStat InType, float Value);

	void ClearRegenTimer();
	void RefreshRegenTimer();
	void SetCurrentValue(float InValue, bool InterruptRegeneration);
	void AddModifier(float Value);
	void RemoveModifier(float Value);

	// start Declare Events.
public:
	DECLARE_EVENT_TwoParams(UExtendedStatComponent, FOnValueChanged, float, float);
	FOnValueChanged& OnValueChanged() { return ValueChangedEvent; }

private:
	FOnValueChanged ValueChangedEvent;
	// end Declare Events.

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
