#include "ExtendedStatComponent.h"
#include "Components/ActorComponent.h"
#include "StatsManagerComponent.h"

#include "Items/ObjectItems/ItemBase.h"

#include "DCSLib.h"
#include "TimerManager.h"
#include "Engine/World.h"

// start public:
UExtendedStatComponent::UExtendedStatComponent()
{
	StatType = EStat::None;

	RegenerationTickInterval = 0.05f;
	CurrentValue = 100.0f;
	TopValue = 100.0f;
	ModifierValue = 0.0f;
	RegenValue = 1.0f;
	ReenableRegenTime = 0.5f;
	InitialRegenValue = 0.0f;
	bDoesRegenerates = false;
}

EStat UExtendedStatComponent::GetStatType() const
{
	return StatType;
}

float UExtendedStatComponent::GetCurrentValue() const
{
	return CurrentValue;
}

float UExtendedStatComponent::GetMaxValue() const
{
	return TopValue + ModifierValue;
}

void UExtendedStatComponent::ModifyStat(float InValue, bool InterruptRegeneration)
{
	if (InValue == 0.0f)
	{
		return;
	}

	if (InterruptRegeneration)
	{
		ClearRegenTimer();
	}
	else
	{
		SetCurrentValue(CurrentValue + InValue, InterruptRegeneration);
		RefreshRegenTimer();
	}
}
// end public:

// start protected:
void UExtendedStatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitStatManager();
}

void UExtendedStatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
// end protected:

void UExtendedStatComponent::InitStatManager()
{
	auto StatsManager = UDCSLib::GetComponent<UStatsManagerComponent>(GetOwner());
	check(StatsManager);

	TopValue = StatsManager->GetStatValue(StatType, false);

	StatsManager->OnModifierAdded().AddUObject(this, &UExtendedStatComponent::OnModifierAdded);
	StatsManager->OnModifierRemoved().AddUObject(this, &UExtendedStatComponent::OnModifierRemoved);
	StatsManager->OnBaseValueChanged().AddUObject(this, &UExtendedStatComponent::OnBaseValueChanged);
}

void UExtendedStatComponent::OnRegenTick()
{
	if (CurrentValue < GetMaxValue())
	{
		ModifyStat(RegenValue, false);
	}
	else
	{
		ClearRegenTimer();
	}
}

void UExtendedStatComponent::OnStartRegenerating()
{
	auto& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(RegenHandle, this, &UExtendedStatComponent::OnRegenTick, RegenerationTickInterval);
}

void UExtendedStatComponent::OnModifierAdded(EStat InType, float Value)
{
	if (StatType != InType)
	{
		return;
	}

	AddModifier(Value);
}

void UExtendedStatComponent::OnModifierRemoved(EStat InType, float Value)
{
	if (StatType != InType)
	{
		return;
	}

	RemoveModifier(Value);
}

void UExtendedStatComponent::OnBaseValueChanged(EStat InType, float Value)
{
	if (StatType != InType)
	{
		return;
	}

	TopValue = Value;
	ValueChangedEvent.Broadcast(CurrentValue, Value);
}

void UExtendedStatComponent::ClearRegenTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RegenHandle);
}

void UExtendedStatComponent::RefreshRegenTimer()
{
	if (bDoesRegenerates == false)
	{
		return;
	}

	auto& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(RegenHandle) == false)
	{
		TimerManager.SetTimer(RegenHandle, this, &UExtendedStatComponent::OnStartRegenerating, false);
	}
}

void UExtendedStatComponent::SetCurrentValue(float InValue, bool InterruptRegeneration)
{
	CurrentValue = FMath::Clamp(InValue, -100.0f, GetMaxValue());
	ValueChangedEvent.Broadcast(CurrentValue, GetMaxValue());

	if (InterruptRegeneration)
	{
		ClearRegenTimer();
		RefreshRegenTimer();
	}
}

void UExtendedStatComponent::AddModifier(float Value)
{
	ModifierValue += Value;

	CurrentValue = FMath::Clamp(CurrentValue, 0.0f, GetMaxValue());
	SetCurrentValue(CurrentValue, false);
	RefreshRegenTimer();
}

void UExtendedStatComponent::RemoveModifier(float Value)
{
	ModifierValue -= Value;

	CurrentValue = FMath::Clamp(CurrentValue, 0.0f, GetMaxValue());
	SetCurrentValue(CurrentValue, false);
	RefreshRegenTimer();
}

void UExtendedStatComponent::ChangeRegenPercent(float InPercent)
{
	RegenValue = (InPercent / 100.0f) * InitialRegenValue;
}
