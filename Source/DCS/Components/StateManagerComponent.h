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
	FORCEINLINE void SetActivity();
	FORCEINLINE void ResetState();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private: 
	TMap<EActivity, bool> Activities;

	EState CurrentState;
};
