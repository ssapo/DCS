#include "DCSAIController.h"
#include "BaseAICharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "Components/EquipmentComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void ADCSAIController::OnPossess(APawn* InPawn)
{
	PossessedAI = Cast<ABaseAICharacter>(InPawn);
	if (PossessedAI.IsValid() == false)
	{
		return;
	}

	RunBehaviorTree(PossessedAI->GetBehaviorTree());

	FTimerManager& TimerMangaer = GetWorld()->GetTimerManager();
	TimerMangaer.ClearTimer(TH_Update);
	TimerMangaer.SetTimer(TH_Update, this, &ADCSAIController::UpdateTarget, 1.0f, true);

	UEquipmentComponent* Equipment = PossessedAI->GetEquipment();
	if (Equipment == nullptr)
	{
		return;
	}

	Equipment->OnCombatChanged().AddUObject(this, &ADCSAIController::OnCombatChanged);
}

void ADCSAIController::UpdateTarget()
{

}

void ADCSAIController::SetTarget(AActor* NewTarget)
{

}

void ADCSAIController::SetIsInCombat(bool InValue)
{
	if (IsInCombat != InValue)
	{
		IsInCombat = InValue;

		if (Blackboard)
		{
			Blackboard->SetValueAsBool(IsInCombatKey, IsInCombat);
		}
	}
}

void ADCSAIController::UpdateSenseTarget()
{
}

void ADCSAIController::OnCombatChanged(bool InValue)
{
	SetIsInCombat(InValue);
}
