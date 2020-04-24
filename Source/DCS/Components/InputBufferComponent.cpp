#include "InputBufferComponent.h"

UInputBufferComponent::UInputBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInputBufferComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInputBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInputBufferComponent::ConsumeInputBuffer()
{
	InputBufferConsumedEvent.Broadcast(StoredKey);

	StoredKey = EInputBufferKey::None;
}

FORCEINLINE void UInputBufferComponent::OpenInputBuffer()
{
	bIsOpen = true;
	InputBufferOpenedEvent.Broadcast();
}

FORCEINLINE void UInputBufferComponent::CloseInputBuffer()
{
	bIsOpen = false;
	InputBufferClosedEvent.Broadcast();
}

void UInputBufferComponent::UpdateKey(EInputBufferKey InputKey)
{
	StoredKey = InputKey;

	if (bIsOpen == false)
	{
		ConsumeInputBuffer();
	}
}

void UInputBufferComponent::StopJumping()
{
	// TODO: fill function
}

