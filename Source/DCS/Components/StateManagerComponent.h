// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "Map.h"
#include "StateManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStateManagerComponent();

	FORCEINLINE bool GetActivityValue(EActivity InType) const;
	FORCEINLINE void SetState(EState InState);
	FORCEINLINE EState GetState() const;
	FORCEINLINE void SetActivity(EActivity InType, bool InValue);
	FORCEINLINE void ResetState(float InTime);

private:
	void SetIdleState();

	// start Declare Events.
public:
	DECLARE_EVENT_TwoParams(UStateManagerComponent, FOnStateChanged, EState, EState);
	FOnStateChanged& OnStateChanged() { return StateChangedEvent; }

	DECLARE_EVENT_TwoParams(UStateManagerComponent, FOnActivityChanged, EActivity, bool);
	FOnActivityChanged& OnActivityChanged() { return ActivityChangedEvent; }

private:
	FOnStateChanged StateChangedEvent;
	FOnActivityChanged ActivityChangedEvent;
	// end Declare Events.

private: 
	TMap<EActivity, bool> Activities;

	EState CurrentState;
	EState PrevState;
	FTimerHandle IdleTimer;
};
