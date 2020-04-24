#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "CollisionHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UCollisionHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCollisionHandlerComponent();

	void ActivateCollision(ECollisionPart InType);
	void DeActivateCollision();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// start Declare Events.
public:
	DECLARE_EVENT(UCollisionHandlerComponent, FOnHit);
	FOnHit& OnHit() { return HitEvent; }

	DECLARE_EVENT_OneParam(UCollisionHandlerComponent, FOnCollisionActivated, ECollisionPart);
	FOnCollisionActivated& OnCollisionActivated() { return CollisionActivatedEvent; }

	DECLARE_EVENT(UCollisionHandlerComponent, FOnCollisionDeActivated);
	FOnCollisionDeActivated& OnCollisionDeActivated() { return CollisionDeActivatedEvent; }

private:
	FOnHit HitEvent;
	FOnCollisionActivated CollisionActivatedEvent;
	FOnCollisionDeActivated CollisionDeActivatedEvent;
	// end Declare Events.

private:
	TArray<FCollisionComponent> CollisionComponents;
	TArray<FCollCompHitActors> HitActors;

	bool bCanPerformTrace;
	bool bCollisionActive;
};
