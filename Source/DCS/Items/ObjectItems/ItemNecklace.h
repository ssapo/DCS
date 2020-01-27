#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemBase.h"
#include "ItemNecklace.generated.h"

class AActor;

UCLASS()
class DCS_API UItemNecklace : public UItemBase
{
	GENERATED_BODY()
	
public:
	virtual void UseItem(AActor* Caller) override;

};
