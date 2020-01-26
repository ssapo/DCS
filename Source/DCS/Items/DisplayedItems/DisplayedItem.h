#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DisplayedItem.generated.h"

UCLASS()
class DCS_API ADisplayedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ADisplayedItem();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
