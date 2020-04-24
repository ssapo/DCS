#include "StatsManagerComponent.h"

#include "Items/ObjectItems/ItemBase.h"

#include "DCSLib.h"

// start public:
UStatsManagerComponent::UStatsManagerComponent()
{
	Stats.Empty();
	Stats.Add(FStat(EStat::Health, 100.0f, 0.0f));
	Stats.Add(FStat(EStat::Stamina, 100.0f, 0.0f));
	Stats.Add(FStat(EStat::Mana, 100.0f, 0.0f));
	Stats.Add(FStat(EStat::Damage, 10.0f, 0.0f));
	Stats.Add(FStat(EStat::Armor, 0.0f, 0.0f));
	Stats.Add(FStat(EStat::CritChance, 0.0f, 0.0f));
	Stats.Add(FStat(EStat::CritMultiplier, 0.0f, 0.0f));
	Stats.Add(FStat(EStat::AttackSpeed, 1.0f, 0.0f));
	Stats.Add(FStat(EStat::Block, 50.0f, 0.0f));
	Stats.Add(FStat(EStat::MeleeAttackStaminaCost, 15.0f, 0.0f));
	Stats.Add(FStat(EStat::MagicDamage, 10.0f, 0.0f));
	Stats.Add(FStat(EStat::CastingSpeed, 1.0f, 0.0f));
}

void UStatsManagerComponent::Initialize()
{
	// TODO: fill function
}

void UStatsManagerComponent::Finalize()
{
	// TODO: fill function
}

float UStatsManagerComponent::GetStatValue(EStat InType, bool IncludeModifiers) const
{
	float RetVal = 0.0f;
	int32 LastIndex = GetStatIndex(InType);
	if (Stats.IsValidIndex(LastIndex) == false)
	{
		return RetVal;
	}

	RetVal = Stats[LastIndex].BaseValue;
	if (IncludeModifiers)
	{
		RetVal += Stats[LastIndex].ModifiersValue;
	}

	return RetVal;
}

int32 UStatsManagerComponent::GetStatIndex(EStat InType) const
{
	int32 RetVal = 0;
	for (const auto& E : Stats)
	{
		if (E.Type == InType)
		{
			return RetVal;
		}
		++RetVal;
	}

	return UDCSLib::INV_INDEX;
}

float UStatsManagerComponent::GetDamage() const
{
	float Damage = GetStatValue(EStat::Damage, true);
	float CritChance = GetStatValue(EStat::CritChance, true);

	float CritMult = 1.0f;
	float RandRangeValue = FMath::RandRange(1.0f, 100.0f);
	if (CritChance > RandRangeValue)
	{
		CritMult = GetStatValue(EStat::CritMultiplier, true);
	}

	return FMath::TruncToInt(Damage * CritMult);
}
// end public:

// start protected:
void UStatsManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}
// end protected: