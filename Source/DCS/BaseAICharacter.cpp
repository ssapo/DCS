#include "BaseAICharacter.h"
#include "DCSLib.h"

#include "Components/StatsManagerComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "Components/EquipmentComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DCSAIController.h"

bool ABaseAICharacter::IsStateEqual(EState InType) const
{
	return CStateManager->GetState() == InType;
}

bool ABaseAICharacter::IsCombatEqual(ECombat InType) const
{
	return CEquipment->GetCombatType() == InType;
}

UBehaviorTree* ABaseAICharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}

UEquipmentComponent* ABaseAICharacter::GetEquipment() const
{
	return CEquipment;
}

bool ABaseAICharacter::OnSelected()
{
	TargetWidget->SetHiddenInGame(true, false);
}

bool ABaseAICharacter::OnDeselected()
{
	TargetWidget->SetHiddenInGame(false, false);
}

bool ABaseAICharacter::IsTargetable() const
{
	return IsAlive();
}

EAttackResult ABaseAICharacter::TakeDamage(const FHitData& HitData)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool ABaseAICharacter::IsAlive() const
{
	return IsStateEqual(EState::Dead);
}

FName ABaseAICharacter::GetHeadSocket() const
{
	return FName("head");
}

bool ABaseAICharacter::CanEffectBeApplied(EEffect InType, AActor* Actor) const
{
	throw std::logic_error("The method or operation is not implemented.");
}

UDataTable* ABaseAICharacter::GetMontages(EMontage InType)
{
	return nullptr;
}

FRotator ABaseAICharacter::GetDesiredRotation() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponents();
}

void ABaseAICharacter::InitializeComponents()
{
	CStatsManager->Initialize();
	CEquipment->Initialize();
	
	BaseAIController = UDCSLib::GetAIController(this);
	check(BaseAIController);
	
	CExtendedHealth->SetCurrentValue(CExtendedHealth->GetMaxValue(), false);
}
