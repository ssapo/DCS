#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "SharedPointer.h"
#include "DataTables.h"
#include "MontageManagerComponent.generated.h"

class IIsMontageManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UMontageManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMontageManagerComponent();
	UAnimMontage* GetMontageForAction(EMontage InType, int32 Index);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FMontageActionData GetMontage(EMontage InAction) const;

private:
	EMontage LastRequestedAction;
	IIsMontageManager* IMontageManager;
};
