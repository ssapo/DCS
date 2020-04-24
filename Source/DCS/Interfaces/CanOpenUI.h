#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanOpenUI.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanOpenUI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API ICanOpenUI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OpenedUI() abstract;
	virtual void ClosedUI() abstract;
};
