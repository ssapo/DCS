// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManagerComponent.h"

// start public:
UStateManagerComponent::UStateManagerComponent()
{

}

FORCEINLINE bool UStateManagerComponent::GetActivityValue(EActivity InType) const
{
	if (Activities.Contains(InType))
	{
		return Activities[InType];
	}
	else
	{
		return false;
	}
}

FORCEINLINE void UStateManagerComponent::SetState(EState InState)
{
	CurrentState = InState;
}

FORCEINLINE EState UStateManagerComponent::GetState() const
{
	return CurrentState;
}

FORCEINLINE void UStateManagerComponent::SetActivity()
{
	// TODO: fill function
}

FORCEINLINE void UStateManagerComponent::ResetState()
{
	// TODO: fill function
}

// end public:

// start protected:
void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// TODO: fill function
}
// end protected:
