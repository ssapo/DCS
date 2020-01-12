#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "Array.h"
#include "MovementSpeedComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UMovementSpeedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMovementSpeedComponent();
	
	void ToggleState();

	FORCEINLINE float GetJogSpeed() const;
	FORCEINLINE EMovementState GetMovementState() const;
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateMaxSpeed();
	void ChangeMovementState(EMovementState InState);
	float GetSelectSpeed(EMovementState InSelector) const;

public:
	DECLARE_EVENT_OneParam(UMovementSpeedComponent, FOnMovementStateEnd, EMovementState);
	FOnMovementStateEnd& OnMovementStateEnd() { return MovementStateEndEvent; }

	DECLARE_EVENT_OneParam(UMovementSpeedComponent, FOnMovementStateStart, EMovementState);
	FOnMovementStateStart& OnMovementStateStart() { return MovementStateStartEvent; }

private:
	TArray<EMovementState> StatesToToggle;

	FOnMovementStateEnd MovementStateEndEvent;
	FOnMovementStateStart MovementStateStartEvent;

	EMovementState MovementState;

	float WalkSpeed;
	float JogSpeed;
	float SprintSpeed;
	float TargetSpeed;
	float SpeedChangeInterpSpeed;

	bool bIsUpdatingSpeed;
};
