#include "RotatingComponent.h"
#include "DCSLib.h"
#include "Interfaces/CanDesiredRotating.h"

URotatingComponent::URotatingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bShouldRotate = false;
}

void URotatingComponent::StartRotatingWithLimit(float InMaxPossibleRotation, float InMaxDegreesPerSecond)
{
	RotateTime = (InMaxPossibleRotation / InMaxDegreesPerSecond);
	MaxDegreesPerSecond = InMaxDegreesPerSecond;
	TimeElapsed = 0.0f;
	bShouldRotate = true;

	RotatingStartEvent.Broadcast();
}

void URotatingComponent::StartRotatingWithTime(float Time, float InMaxDegreesPerSecond)
{
	RotateTime = Time;
	MaxDegreesPerSecond = InMaxDegreesPerSecond;
	TimeElapsed = 0.0f;
	bShouldRotate = true;

	RotatingStartEvent.Broadcast();
}

void URotatingComponent::StopRotating()
{
	bShouldRotate = false;
	RotatingEndEvent.Broadcast();
}

void URotatingComponent::SetRotationMode(ERotationMode Mode)
{
	// TODO: fill function
}

FORCEINLINE bool URotatingComponent::IsRotating() const
{
	return bShouldRotate;
}

void URotatingComponent::BeginPlay()
{
	Super::BeginPlay();

	check(UDCSLib::IsInterface(GetOwner(), UCanDesiredRotating::StaticClass()));
	
	IDesiredRotating = Cast<ICanDesiredRotating>(GetOwner());
}

void URotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldRotate == false)
	{
		return;
	}

	TimeElapsed += UDCSLib::GetDTS(this);
	if (TimeElapsed <= RotateTime)
	{
		FRotator ActorRot = GetOwner()->GetActorRotation();

		FRotator DesiredRot = IDesiredRotating->GetDesiredRotation();

		FRotator Rot = UDCSLib::InterpTo(ActorRot, DesiredRot, UDCSLib::GetDTS(this), MaxDegreesPerSecond);

		GetOwner()->SetActorRotation(Rot);
	}
	else 
	{
		StopRotating();
	}
}
