#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemBase.h"
#include "ItemArmor.generated.h"

UCLASS(Blueprintable)
class DCS_API UItemArmor : public UItemBase
{
	GENERATED_BODY()
	
public:
	void UseItem(AActor* Caller) override;

};
