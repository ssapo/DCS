#pragma once

#include "CoreMinimal.h"
#include "Items/ObjectItems/ItemBase.h"
#include "Interfaces/ItemCanBlock.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "ItemShield.generated.h"

class AActor;

UCLASS(Blueprintable)
class DCS_API UItemShield : public UItemBase
	, public IItemCanBlock
	, public IItemIsDisplayed
{
	GENERATED_BODY()
	
public:
	float GetBlockValue() const override;
	const TSubclassOf<ADisplayedItem>& GetDisplayedItem() const override;

protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ADisplayedItem> DisplayedItemClass;

	UPROPERTY(EditAnywhere)
		float BlockValue;



};
