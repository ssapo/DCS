#include "BaseAICharacter.h"
#include "DCSLib.h"

#include "Components/StatsManagerComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"

UBehaviorTree* ABaseAICharacter::GetBehaviorTree() const
{
	return BehaviorTree;
}

UEquipmentComponent* ABaseAICharacter::GetEquipment() const
{
	return CEquipment;
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponents();
}

void ABaseAICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseAICharacter::InitializeComponents()
{
	CStatsManager->Initialize();
	CStateManager->Initialize();

	auto AIController = UDCSLib::GetAIController(this);

	CExtendedHealth->SetCurrentValue(CExtendedHealth->GetMaxValue(), false);
}
