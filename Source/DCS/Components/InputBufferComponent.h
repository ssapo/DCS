#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "InputBufferComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInputBufferComponent();

	void UpdateKey(EInputBufferKey InputKey);
	void StopJumping();
	void ConsumeInputBuffer();

	FORCEINLINE void OpenInputBuffer();
	FORCEINLINE void CloseInputBuffer();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	DECLARE_EVENT_OneParam(UInputBufferComponent, FOnInputBufferConsumed, EInputBufferKey);
	FOnInputBufferConsumed& OnInputBufferConsumed() { return InputBufferConsumedEvent; }

	DECLARE_EVENT(UInputBufferComponent, FOnInputBufferOpened);
	FOnInputBufferOpened& OnInputBufferOpened() { return InputBufferOpenedEvent; }

	DECLARE_EVENT(UInputBufferComponent, FOnInputBufferClosed);
	FOnInputBufferClosed& OnInputBufferClosed() { return InputBufferClosedEvent; }

private:
	FOnInputBufferConsumed InputBufferConsumedEvent;
	FOnInputBufferOpened InputBufferOpenedEvent;
	FOnInputBufferClosed InputBufferClosedEvent;

	EInputBufferKey StoredKey;

	bool bIsOpen;
};
