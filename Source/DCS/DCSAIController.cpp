#include "DCSAIController.h"
#include "BaseAICharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "Components/EquipmentComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ADCSAIController::ADCSAIController()
{
	CtorComponents();

	CtorInitialize();
}

void ADCSAIController::CtorComponents()
{
	CAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	ensure(CAIPerception != nullptr);
}

void ADCSAIController::CtorInitialize()
{
	TargetKey = TEXT("Target");
	AttackTypeKey = TEXT("AttackType");
	StateKey = TEXT("State");
	IsInCombatKey = TEXT("IsInCombat");

	IsInCombat = false;
}

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
