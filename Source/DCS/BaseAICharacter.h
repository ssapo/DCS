#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NameTypes.h"
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

enum class EMeleeAttack : uint8;
enum class EDirection : uint8;

UCLASS()
class DCS_API ABaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseAICharacter();

	UBehaviorTree* GetBehaviorTree() const;
	UEquipmentComponent* GetEquipment() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void CtorComponents();
	void CtorInitialize();
	void InitializeComponents();

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UCollisionHandlerComponent* CMeleeCollisionHandler;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UExtendedStatComponent* CExtendedHealth;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UExtendedStatComponent* CExtendedStamina;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UMontageManagerComponent* CMontagesManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UBehaviorComponent* CBehavior;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UEffectsComponent* CEffects;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UDissolveComponent* CDissolveEffect;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UEquipmentComponent* CEquipment;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		URotatingComponent* CRotating;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UStateManagerComponent* CStateManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UStatsManagerComponent* CStatsManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UPatrolComponent* CPatrol;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UMovementSpeedComponent* CMovementSpeed;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FName> LeftFootCollisionSockets;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FName> RightFootCollisionSockets;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FName> LeftHandCollisionSockets;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		TArray<FName> RightHandCollisionSockets;

	EDirection ReceivedHitDirection;
	EMeleeAttack MeleeAttackType;
	mutable int32 MeleeCounterAttack;
	float RecentlyReceivedDamageStunLimit;
	int32 RecentlyReceivedHitsCountStunLimit;
};
