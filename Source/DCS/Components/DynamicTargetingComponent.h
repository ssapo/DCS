#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicTargetingComponent.generated.h"

class UArrowComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UDynamicTargetingComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UDynamicTargetingComponent();

	void Initialize(const UArrowComponent& Arrow);
	void FindTargetWithAxisInput(float InAxisValue);

	bool IsTargetingEnabled() const;
	AActor* GetSelectedActor() const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bIsTargetingEnabled;
	AActor* SelectedActor;
};
