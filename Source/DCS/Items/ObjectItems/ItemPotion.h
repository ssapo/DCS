#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemBase.h"
#include "ItemPotion.generated.h"

class AActor;

UCLASS(Blueprintable)
class DCS_API UItemPotion : public UItemBase
{
	GENERATED_BODY()
	
public:
	virtual void UseItem(AActor* Caller) override;

};
