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

protected:
	virtual void OnPossess(APawn* InPawn) override;

	void UpdateTarget();
	void SetTarget(AActor* NewTarget);
	void SetIsInCombat(bool InValue);
	void UpdateSenseTarget();

private:
	void OnCombatChanged(bool InValue);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		FName TargetKey;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		FName AttackTypeKey;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		FName StateKey;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		FName IsInCombatKey;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UAIPerceptionComponent* AIPerception;

	UPROPERTY(Transient)
		TWeakObjectPtr<ABaseAICharacter> PossessedAI;

	UPROPERTY(Transient)
		AActor* Target;

	FTimerHandle TH_Update;

	bool IsInCombat;
};
