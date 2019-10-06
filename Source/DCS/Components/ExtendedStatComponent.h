// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "Components/ActorComponent.h"
#include "ExtendedStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UExtendedStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExtendedStatComponent(); 

	EStat GetStatType() const { return StatType; }
	float GetCurrentValue() const { return CurrentValue; }

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
