

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SubclassOf.h"
#include "ItemIsDisplayed.generated.h"

class ADisplayedItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemIsDisplayed : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API IItemIsDisplayed
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual const TSubclassOf<ADisplayedItem>& GetDisplayedItem() const abstract;
};
