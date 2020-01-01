#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DCSInstance.generated.h"

UCLASS()
class DCS_API UDCSInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Shutdown() override;
	void StartGameInstance() override;
};
