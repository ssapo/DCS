

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs.h"
#include "CanGetEffects.generated.h"

class AActor;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanGetEffects : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API ICanGetEffects
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanEffectBeApplied(EEffect InType, AActor* Actor) const abstract;
};
