#include "CollisionHandlerComponent.h"

UCollisionHandlerComponent::UCollisionHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bCanPerformTrace = false;
	bCollisionActive = false;
}

void UCollisionHandlerComponent::ActivateCollision(ECollisionPart InType)
{
	HitActors.Empty();

	bCollisionActive = true;

	CollisionActivatedEvent.Broadcast(InType);
}

void UCollisionHandlerComponent::DeActivateCollision()
{
	bCollisionActive = false;
	bCanPerformTrace = false;

	CollisionDeActivatedEvent.Broadcast();
}

void UCollisionHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCollisionHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

