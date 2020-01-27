#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemBase.h"
#include "ItemRing.generated.h"

class AActor;

UCLASS(Blueprintable)
class DCS_API UItemRing : public UItemBase
{
	GENERATED_BODY()
	
public:
	virtual void UseItem(AActor* Caller) override;

};
