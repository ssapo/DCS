#include "RotatingComponent.h"

URotatingComponent::URotatingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
}

void URotatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


