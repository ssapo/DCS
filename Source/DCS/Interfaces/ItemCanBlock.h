

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemCanBlock.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemCanBlock : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API IItemCanBlock
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetBlockValue() const abstract;
};
