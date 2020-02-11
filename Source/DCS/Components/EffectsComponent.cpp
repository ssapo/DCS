#include "EffectsComponent.h"

#include "Interfaces/CanGetEffects.h"

#include "DCSLib.h"

// start public:
UEffectsComponent::UEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UEffectsComponent::ApplyBackstabEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage)
{
	if (ICanGetEffects* Owner = Cast<ICanGetEffects>(GetOwner()))
	{
		if (Owner->CanEffectBeApplied(EEffect::Backstab, Applier) == false)
		{
			return false;
		}
	}

	BackstabDamage = Damage;

	UpdateEffect(EEffect::Backstab, Duration, Method, Applier);

	EffectAppliedEvent.Broadcast(EEffect::Backstab);

	return true;
}
// end public:

// start protected:
void UEffectsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
// end protected:

// start private:
void UEffectsComponent::UpdateEffect(EEffect Type, float Duration, EApplyEffectMethod Method, AActor* Applier)
{
	// TODO: fill function
}
// end private:
