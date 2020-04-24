// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "UObject/Interface.h"
#include "IsItemsContainer.generated.h"

class UUserWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsItemsContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DCS_API IIsItemsContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsOpen() const abstract;
	virtual bool ItemClicked(UUserWidget* Inventory) abstract;
	virtual bool UpdateItemsSlots(EItem InType) abstract;
	virtual bool CreateItemsSlots() abstract;
};
