#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structs.h"
#include "IsMontageManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsMontageManager : public UInterface
{
	GENERATED_BODY()
};

class UDataTable;

class DCS_API IIsMontageManager
{
	GENERATED_BODY()

public:
	virtual UDataTable* GetMontages(EMontage InType) abstract;
};
