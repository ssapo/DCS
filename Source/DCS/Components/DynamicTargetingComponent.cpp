#include "DynamicTargetingComponent.h"

UDynamicTargetingComponent::UDynamicTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDynamicTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDynamicTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDynamicTargetingComponent::Initialize(const UArrowComponent& Arrow)
{
	// TODO: fill function
}

void UDynamicTargetingComponent::FindTargetWithAxisInput(float InAxisValue)
{
	// TODO: fill function
}

bool UDynamicTargetingComponent::IsTargetingEnabled() const
{
	return bIsTargetingEnabled;
}

AActor* UDynamicTargetingComponent::GetSelectedActor() const
{
	return SelectedActor;
}

