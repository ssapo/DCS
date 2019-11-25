

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs.h"
#include "CanBeAttacked.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanBeAttacked : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API ICanBeAttacked
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EAttackResult TakeDamage(const FHitData& HitData) abstract;
	virtual bool IsAlive() const abstract;
	virtual FName GetHeadSocket() const abstract;
};
