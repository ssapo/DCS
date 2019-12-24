#include "DCSGameModeBase.h"
#include "CombatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryComponent.h"
#include <Components/ActorComponent.h>
#include "Array.h"

ADCSGameModeBase::ADCSGameModeBase()
{
	DefaultPawnClass = ACombatCharacter::StaticClass();
}

void ADCSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UpdateInventoryValues();

	UpdateEquipmentValues();

	UpdateStatValues();
}

void ADCSGameModeBase::LoadGame()
{

}

void ADCSGameModeBase::SaveGame()
{

}

void ADCSGameModeBase::UpdateEquipmentValues()
{
	auto Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	verify(Character != nullptr);

	auto Components = Character->GetComponentsByClass(UExtendedStatComponent::StaticClass());
	verify(Components.Num() > 0);

	for (auto Element : Components)
	{
		if (auto ExtendedStat = Cast<UExtendedStatComponent>(Element))
		{
			EStat StatType = ExtendedStat->GetStatType();
			float CurrentValue = ExtendedStat->GetCurrentValue();

			CurrentStatValues.Add(StatType, CurrentValue);
		}
	}
}

void ADCSGameModeBase::UpdateInventoryValues()
{
	auto Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	verify(Character != nullptr);

	auto EquipmentComponent = Character->FindComponentByClass<UEquipmentComponent>();
	verify(EquipmentComponent != nullptr);

	EquipmentSlots = EquipmentComponent->GetEquipmentSlots();
	bIsInCombat = EquipmentComponent->IsInCombat();
	SelectedMainHandSlotType = EquipmentComponent->GetSelectedMainHandType();
}

void ADCSGameModeBase::UpdateStatValues()
{
	auto Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	verify(Character != nullptr);

	auto InventoryComponent = Character->FindComponentByClass<UInventoryComponent>();
	verify(InventoryComponent != nullptr);

	Inventory = InventoryComponent->GetInventory();
}

