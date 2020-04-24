#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "Components/ActorComponent.h"
#include "StatsManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UStatsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsManagerComponent();
	
	void Initialize();
	void Finalize();

	float GetStatValue(EStat InType, bool IncludeModifiers) const;
	int32 GetStatIndex(EStat InType) const;
	float GetDamage() const;

protected:
	virtual void BeginPlay() override;

	// start Declare Events.
public:
	DECLARE_EVENT_TwoParams(UExtendedStatComponent, FOnModifierAdded, EStat, float);
	FOnModifierAdded& OnModifierAdded() { return ModifierAddedEvent; }
	
	DECLARE_EVENT_TwoParams(UExtendedStatComponent, FOnModifierRemoved, EStat, float);
	FOnModifierRemoved& OnModifierRemoved() { return ModifierRemovedEvent; }
	
	DECLARE_EVENT_TwoParams(UExtendedStatComponent, FOnBaseValueChanged, EStat, float);
	FOnBaseValueChanged& OnBaseValueChanged() { return BaseValueChangedEvent; }

private:
	FOnModifierAdded ModifierAddedEvent;
	FOnModifierRemoved ModifierRemovedEvent;
	FOnBaseValueChanged BaseValueChangedEvent;
	// end Declare Events.

private:
	TArray<FStat> Stats;
};
