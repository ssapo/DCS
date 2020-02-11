#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs.h"
#include "EffectsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEffectsComponent();
	bool ApplyBackstabEffect(float Duration, EApplyEffectMethod Method, AActor* Actor, float Damage);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateEffect(EEffect Type, float Duration, EApplyEffectMethod Method, AActor* Applier);

	// start Declare Events.
public:
	DECLARE_EVENT_OneParam(UEffectsComponent, FOnEffectApplied, EEffect);
	FOnEffectApplied& OnEffectApplied() { return EffectAppliedEvent; }

	DECLARE_EVENT_OneParam(UEffectsComponent, FOnEffectRemoved, EEffect);
	FOnEffectRemoved& OnEffectRemoved() { return EffectRemovedEvent; }

private:
	FOnEffectApplied EffectAppliedEvent;
	FOnEffectRemoved EffectRemovedEvent;
	// end Declare Events.

private:
	float BackstabDamage;
		
};
