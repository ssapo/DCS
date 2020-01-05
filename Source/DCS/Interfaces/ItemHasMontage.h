// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemHasMontage.generated.h"

class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemHasMontage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API IItemHasMontage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UAnimMontage* GetUseMontage() const abstract;
};
