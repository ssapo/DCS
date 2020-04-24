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
	void SetMovementState(EMovementState InState);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateMaxSpeed();
	void ChangeMovementState(EMovementState InState);
	float GetSelectSpeed(EMovementState InSelector) const;

	// start declare events.
public:
	DECLARE_EVENT_OneParam(UMovementSpeedComponent, FOnMovementStateEnd, EMovementState);
	FOnMovementStateEnd& OnMovementStateEnd() { return MovementStateEndEvent; }

	DECLARE_EVENT_OneParam(UMovementSpeedComponent, FOnMovementStateStart, EMovementState);
	FOnMovementStateStart& OnMovementStateStart() { return MovementStateStartEvent; }

private:
	FOnMovementStateEnd MovementStateEndEvent;
	FOnMovementStateStart MovementStateStartEvent;
	// end declare events.

private:
	UPROPERTY(EditInstanceOnly)
		float WalkSpeed;

	UPROPERTY(EditInstanceOnly)
		float JogSpeed;

	UPROPERTY(EditInstanceOnly)
		float SprintSpeed;

	UPROPERTY(EditInstanceOnly)
		float TargetSpeed;

	UPROPERTY(EditInstanceOnly)
		float SpeedChangeInterpSpeed;
	
	UPROPERTY(EditInstanceOnly)
		TArray<EMovementState> StatesToToggle;

	EMovementState MovementState;
	bool bIsUpdatingSpeed;
};
