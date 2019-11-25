#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsTargetable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsTargetable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API IIsTargetable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool OnSelected() abstract;
	virtual bool OnDeselected() abstract;
	virtual bool IsTargetable() const abstract;
};
