#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "RotatingComponent.generated.h"

class ICanDesiredRotating;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API URotatingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URotatingComponent();

	void StartRotatingWithLimit(float InMaxPossibleRotation, float InMaxDegreesPerSecond);
	void StartRotatingWithTime(float Time, float InMaxDegreesPerSecond);
	void StopRotating();
	
	void SetRotationMode(ERotationMode Mode);

	FORCEINLINE bool IsRotating() const;

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// start Declare Events.
public:
	DECLARE_EVENT(URotatingComponent, FOnRotatingStart);
	FOnRotatingStart& OnRotatingStart() { return RotatingStartEvent; }

	DECLARE_EVENT(URotatingComponent, FOnRotatingEnd);
	FOnRotatingEnd& OnRotatingEnd() { return RotatingEndEvent; }

private:
	FOnRotatingStart RotatingStartEvent;
	FOnRotatingEnd RotatingEndEvent;
	// end Declare Events.

private:
	ICanDesiredRotating* IDesiredRotating;

	float TimeElapsed;
	float RotateTime;
	float MaxDegreesPerSecond;
	float MaxAllowedDegrees;

	bool bShouldRotate;
};
