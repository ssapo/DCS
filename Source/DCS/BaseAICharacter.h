#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NameTypes.h"
#include "Interfaces/IsTargetable.h"
#include "Interfaces/CanBeAttacked.h"
#include "Interfaces/CanGetEffects.h"
#include "Interfaces/IsMontageManager.h"
#include "Interfaces/CanDesiredRotating.h"
#include "Enumerations.h"
#include "BaseAICharacter.generated.h"

class UCollisionHandlerComponent;
class UExtendedStatComponent;
class UMontageManagerComponent;
class UBehaviorComponent;
class UEffectsComponent;
class UDissolveComponent;
class UEquipmentComponent;
class URotatingComponent;
class UStateManagerComponent;
class UStatsManagerComponent;
class UPatrolComponent;
class UMovementSpeedComponent;
class UBehaviorTree;
class ABaseAICharacter;
class ADCSAIController;
class UDataTable;
class UWidgetComponent;

UCLASS()
class DCS_API ABaseAICharacter 
	: public ACharacter
	, public IIsTargetable
	, public ICanBeAttacked
	, public ICanGetEffects
	, public IIsMontageManager
	, public ICanDesiredRotating
{
	GENERATED_BODY()

public:
	ABaseAICharacter();

	bool IsStateEqual(EState InType) const;
	bool IsCombatEqual(ECombat InType) const;

	UBehaviorTree* GetBehaviorTree() const;
	UEquipmentComponent* GetEquipment() const;

protected:
	virtual void BeginPlay() override;

	void CtorComponents();
	void CtorInitialize();
	void InitializeComponents();

	// Start interfaces
	virtual bool OnSelected() override;
	virtual bool OnDeselected() override;
	virtual bool IsTargetable() const override;
	virtual EAttackResult TakeDamage(const FHitData& HitData) override;
	virtual bool IsAlive() const override;
	virtual FName GetHeadSocket() const override;
	virtual bool CanEffectBeApplied(EEffect InType, AActor* Actor) const override;
	virtual UDataTable* GetMontages(EMontage InType) override;
	virtual FRotator GetDesiredRotation() const override;
	// End interfaces

protected:
	UPROPERTY(VisibleAnywhere)
		UWidgetComponent* CStatBarsWidget;

	UPROPERTY(VisibleAnywhere)
		UWidgetComponent* CTargetWidget;

	UPROPERTY(VisibleAnywhere)
		UCollisionHandlerComponent* CMeleeCollisionHandler;

	UPROPERTY(VisibleAnywhere)
		UExtendedStatComponent* CExtendedHealth;

	UPROPERTY(VisibleAnywhere)
		UExtendedStatComponent* CExtendedStamina;

	UPROPERTY(VisibleAnywhere)
		UMontageManagerComponent* CMontagesManager;

	UPROPERTY(VisibleAnywhere)
		UBehaviorComponent* CBehavior;

	UPROPERTY(VisibleAnywhere)
		UEffectsComponent* CEffects;

	UPROPERTY(VisibleAnywhere)
		UDissolveComponent* CDissolveEffect;

	UPROPERTY(VisibleAnywhere)
		UEquipmentComponent* CEquipment;

	UPROPERTY(VisibleAnywhere)
		URotatingComponent* CRotating;

	UPROPERTY(VisibleAnywhere)
		UStateManagerComponent* CStateManager;

	UPROPERTY(VisibleAnywhere)
		UStatsManagerComponent* CStatsManager;

	UPROPERTY(VisibleAnywhere)
		UPatrolComponent* CPatrol;

	UPROPERTY(VisibleAnywhere)
		UMovementSpeedComponent* CMovementSpeed;

	UPROPERTY(EditAnywhere)
		UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditAnywhere)
		TArray<FName> LeftFootCollisionSockets;

	UPROPERTY(EditAnywhere)
		TArray<FName> RightFootCollisionSockets;

	UPROPERTY(EditAnywhere)
		TArray<FName> LeftHandCollisionSockets;

	UPROPERTY(EditAnywhere)
		TArray<FName> RightHandCollisionSockets;

	UPROPERTY(Transient)
		ADCSAIController* BaseAIController;

	UPROPERTY(Transient)
		EDirection ReceivedHitDirection;

	UPROPERTY(Transient)
		EMeleeAttack MeleeAttackType;

	UPROPERTY(Transient)
		mutable int32 MeleeCounterAttack;

	UPROPERTY(Transient)
		float RecentlyReceivedDamageStunLimit;

	UPROPERTY(Transient)
		int32 RecentlyReceivedHitsCountStunLimit;
};
