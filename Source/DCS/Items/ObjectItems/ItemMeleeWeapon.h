#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemWeapon.h"
#include "ItemMeleeWeapon.generated.h"

UCLASS(Blueprintable)
class DCS_API UItemMeleeWeapon : public UItemWeapon
{
	GENERATED_BODY()
	
public:
	virtual void UseItem(AActor* Caller) override;

};
