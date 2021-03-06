#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemBase.h"
#include "ItemWeapon.generated.h"

class AActor;

UCLASS(Blueprintable)
class DCS_API UItemWeapon : public UItemBase
{
	GENERATED_BODY()
	
public:
	FORCEINLINE EWeapon GetWeaponType() const { return WeaponType; }

private:
	UPROPERTY(EditAnywhere)
		EWeapon WeaponType;
};
