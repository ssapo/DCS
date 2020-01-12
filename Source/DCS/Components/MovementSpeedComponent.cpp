#include "MovementSpeedComponent.h"
#include "DCSLib.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatCharacter.h"

UMovementSpeedComponent::UMovementSpeedComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	StatesToToggle.Empty();
	StatesToToggle.Add(EMovementState::Walk);
	StatesToToggle.Add(EMovementState::Jog);

	MovementState = EMovementState::Idle;
	WalkSpeed = 200.0f;
	JogSpeed = 400.0f;
	SprintSpeed = 550.0f;
	TargetSpeed = 0.0f;
	SpeedChangeInterpSpeed = 3.0f;
	bIsUpdatingSpeed = false;
}

void UMovementSpeedComponent::ToggleState()
{
	check(StatesToToggle.Num() > 0);
	int32 LastStateIndex = StatesToToggle.Find(MovementState);
	LastStateIndex = ((LastStateIndex + 1) % StatesToToggle.Num());
	ChangeMovementState(StatesToToggle[LastStateIndex]);
}

FORCEINLINE float UMovementSpeedComponent::GetJogSpeed() const
{
	return JogSpeed;
}

FORCEINLINE EMovementState UMovementSpeedComponent::GetMovementState() const
{
	return MovementState;
}

void UMovementSpeedComponent::BeginPlay()
{
	Super::BeginPlay();

	MovementState = EMovementState::Jog;

	ChangeMovementState(MovementState);
}

void UMovementSpeedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateMaxSpeed();
}

void UMovementSpeedComponent::UpdateMaxSpeed()
{
	if (bIsUpdatingSpeed == false)
	{
		return;
	}
	
	if (auto MovementComponent = UDCSLib::GetComponent<UCharacterMovementComponent>(GetOwner()))
	{
		float MWS = MovementComponent->MaxWalkSpeed;
		MWS = FMath::FInterpTo(MWS, TargetSpeed, UDCSLib::GetDTS(this), SpeedChangeInterpSpeed);
		MovementComponent->MaxWalkSpeed = MWS;

		if (FMath::IsNearlyEqual(MWS, TargetSpeed, 1.0f))
		{
			MovementComponent->MaxWalkSpeed = TargetSpeed;
			bIsUpdatingSpeed = false;
		}
	}

}

void UMovementSpeedComponent::ChangeMovementState(EMovementState InState)
{
	auto TempMC = UDCSLib::GetComponent<UCharacterMovementComponent>(GetOwner());
	if (TempMC)
	{
		MovementStateEndEvent.Broadcast(MovementState);
		MovementState = InState;
		TargetSpeed = GetSelectSpeed(MovementState);
		bIsUpdatingSpeed = true;
		MovementStateStartEvent.Broadcast(MovementState);
	}
}

float UMovementSpeedComponent::GetSelectSpeed(EMovementState InSelector) const
{
	float RetVal = 0.0f;
	switch (InSelector)
	{
	case EMovementState::Walk:
		RetVal = WalkSpeed;
		break;
	case EMovementState::Jog:
		RetVal = JogSpeed;
		break;
	case EMovementState::Sprint:
		RetVal = SprintSpeed;
		break;
	}

	return RetVal;
}
