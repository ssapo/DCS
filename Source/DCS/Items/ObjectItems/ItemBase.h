// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs.h"
#include "DelegateCombinations.h"
#include "ItemBase.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseItem, AActor*, Caller);


UCLASS()
class DCS_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	const FItem& GetItemRef() const;

public:
	FOnUseItem OnUseItem;

private:
	FItem Item;
};
