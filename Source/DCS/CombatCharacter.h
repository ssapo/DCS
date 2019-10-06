// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structs.h"
#include "CombatCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEquipmentComponent;
class UInventoryComponent;
class URotatingComponent;
class UDissolveComponent;
class UTimelineComponent;
class UStateManagerComponent;
class UMontageManagerComponent;
class UMovementSpeedComponent;
class UInputBufferComponent;
class UExtendedStatComponent;
class UCollisionHandlerComponent;
class UEffectsComponent;
class UStatsManagerComponent;
class UDynamicTargetingComponent;

UCLASS()
class DCS_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	void CtorComponents();
	void CtorInitialize();

public:
	void UpdateAimAlpha();
	void CreateKeyBindings();

private:
	TWeakObjectPtr<USpringArmComponent> CameraBoom;
	TWeakObjectPtr<UCameraComponent> FollowCamera;
	TWeakObjectPtr<UAudioComponent> EffectsAudio;
	TWeakObjectPtr<UEffectsComponent> Effects;
	TWeakObjectPtr<URotatingComponent> Rotating;
	TWeakObjectPtr<UEquipmentComponent> Equipment;
	TWeakObjectPtr<UInventoryComponent> Inventory;
	TWeakObjectPtr<UDissolveComponent> Dissolve;
	TWeakObjectPtr<UMovementSpeedComponent> MovementSpeed;
	TWeakObjectPtr<UStatsManagerComponent> StatsManager;
	TWeakObjectPtr<UMontageManagerComponent> MontagesManager;
	TWeakObjectPtr<UStateManagerComponent> StateManager;
	TWeakObjectPtr<UInputBufferComponent> InputBuffer;
	TWeakObjectPtr<USceneComponent> ArrowSpawnLocation;
	TWeakObjectPtr<UExtendedStatComponent> ExtendedStamina;
	TWeakObjectPtr<UExtendedStatComponent> ExtendedHealth;
	TWeakObjectPtr<UCollisionHandlerComponent> MeleeCollisionHandler;
	TWeakObjectPtr<UArrowComponent> TargetingArrow;
	TWeakObjectPtr<UDynamicTargetingComponent> DynamicTargeting;

	TWeakObjectPtr<AActor> BackstabbedActor;
	TWeakObjectPtr<AActor> InteractionActor;

	TArray<FName> LeftHandCollisionSockets;
	TArray<FName> RightHandCollisionSockets;
	TArray<FName> RightFootCollisionSockets;
	TArray<FName> LeftFootCollisionSockets;

	EMeleeAttack MeleeAttackType;
	EDirection ReceivedHitDirection;
	EMovementState StoredMovementState;

	float BlockAlpha;
	float HorizontalLockRate;
	float VerticalLookRate;
	float RollStaminaCost;
	float SprintStaminaCost;
	float AimAlpha;
	float SlowMotionTimeDuration;
	float SlowMotionStaminaCost;
	float TargetLagSpeed;
	float ZoomAlpha;
	float ZoomCameraArmLength;
	float InitialCameraArmLength;
	float InitialCameraLagSpeed;

	int32 MeleeAttackCounter;

	bool bAutoZoom;
	bool bIsCrossHairVisible;
	bool bIsInSlowMotion;
};
