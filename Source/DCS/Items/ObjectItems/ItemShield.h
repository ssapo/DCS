#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemBase.h"
#include "ItemShield.generated.h"

class AActor;

UCLASS(Blueprintable)
class DCS_API UItemShield : public UItemBase
{
	GENERATED_BODY()
	
public:
	virtual void UseItem(AActor* Caller) override;

};
