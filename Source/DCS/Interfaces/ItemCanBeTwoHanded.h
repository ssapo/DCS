

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemCanBeTwoHanded.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemCanBeTwoHanded : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API IItemCanBeTwoHanded
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsTwoHanded() const abstract;
};
