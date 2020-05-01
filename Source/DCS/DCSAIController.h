#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NameTypes.h"
#include "WeakObjectPtrTemplates.h"
#include "DCSAIController.generated.h"

class UAIPerceptionComponent;
class ABaseAICharacter;
class AActor;

UCLASS()
class DCS_API ADCSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADCSAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	void UpdateTarget();
	void SetTarget(AActor* NewTarget);
	void SetIsInCombat(bool InValue);
	void UpdateSenseTarget();

private:
	void CtorComponents();
	void CtorInitialize();
	void OnCombatChanged(bool InValue);

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UAIPerceptionComponent* CAIPerception;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		FName TargetKey;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		FName AttackTypeKey;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		FName StateKey;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		FName IsInCombatKey;

	UPROPERTY(Transient)
		TWeakObjectPtr<ABaseAICharacter> PossessedAI;

	UPROPERTY(Transient)
		AActor* Target;

	FTimerHandle TH_Update;

	bool IsInCombat;
};
