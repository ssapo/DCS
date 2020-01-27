#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs.h"
#include "ItemBase.generated.h"

class AActor;

UCLASS()
class DCS_API UItemBase : public UObject
{
	GENERATED_BODY()
		
public:
	virtual void UseItem(AActor* Caller) {};

	const FItem& GetItemRef() const;

private:
	FItem Item;
};
