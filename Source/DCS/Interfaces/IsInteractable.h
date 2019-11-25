

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsInteractable.generated.h"

class AActor;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API IIsInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(AActor* Actor) abstract;
	virtual FName GetInteractionMessage() const abstract;
};
