#include "StateManagerComponent.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

// start public:
UStateManagerComponent::UStateManagerComponent()
{
	CurrentState = EState::Idle;
}

void UStateManagerComponent::Initialize()
{
	// TODO: fill function
}

void UStateManagerComponent::Finalize()
{
	// TODO: fill function
}

bool UStateManagerComponent::GetActivityValue(EActivity InType) const
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

void UStateManagerComponent::SetActivity(EActivity InType, bool NewValue)
{
	bool PrevValue = GetActivityValue(InType);

	if (NewValue == PrevValue)
	{
		return;
	}

	Activities.Add(InType, NewValue);

	ActivityChangedEvent.Broadcast(InType, NewValue);
}

void UStateManagerComponent::SetState(EState InState)
{
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);

	EState PreviousState = CurrentState;
	CurrentState = InState;

	if (PreviousState != CurrentState)
	{
		StateChangedEvent.Broadcast(PreviousState, CurrentState);
	}
}

EState UStateManagerComponent::GetState() const
{
	return CurrentState;
}

void UStateManagerComponent::ResetState(float InTime)
{
	if (CurrentState == EState::Dead)
	{
		return;
	}

	if (InTime <= 0.0f)
	{
		SetIdleState();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
		GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &UStateManagerComponent::SetIdleState, InTime);
	}
}
// end public:

// start private:
void UStateManagerComponent::SetIdleState()
{
	SetState(EState::Idle);
}
// end private: