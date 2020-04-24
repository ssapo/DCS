

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsArcher.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsArcher : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API IIsArcher
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAimAlpha() const abstract;
	virtual bool DoesHoldBowString() abstract;
	virtual FName GetBowStringSocketName() const abstract;
};
