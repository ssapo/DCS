#include "DisplayedItem.h"

ADisplayedItem::ADisplayedItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADisplayedItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADisplayedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

