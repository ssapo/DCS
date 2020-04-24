#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemWeapon.h"
#include "Interfaces/ItemCanBeTwoHanded.h"
#include "Interfaces/ItemCanBlock.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "Interfaces/ItemHasModifiers.h"
#include "Interfaces/ItemHasAbility.h"
#include "SubclassOf.h"
#include "ItemMeleeWeapon.generated.h"

class ADisplayedItem;

UCLASS(Blueprintable)
class DCS_API UItemMeleeWeapon : public UItemWeapon
	, public IItemCanBeTwoHanded
	, public IItemCanBlock
	, public IItemIsDisplayed
	, public IItemHasModifiers
	, public IItemHasAbility
{
	GENERATED_BODY()
	
public:
	bool IsTwoHanded() const override;
	float GetBlockValue() const override;
	const TSubclassOf<ADisplayedItem>& GetDisplayedItem() const override;
	const TArray<FModifier>& GetModifiers() const override;

protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ADisplayedItem> DisplayedItemClass;

	UPROPERTY(EditAnywhere)
		TArray<FModifier> Modifiers;

	UPROPERTY(EditAnywhere)
		float BlockValue;

	UPROPERTY(EditAnywhere)
		bool bTwoHanded;
};
