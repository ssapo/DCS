// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enumerations.h"
#include "WeakObjectPtrTemplates.h"
#include "CombatAnimInstance.generated.h"

class ACombatCharacter;
class UEquipmentComponent;

/**
 * 
 */
UCLASS()
class DCS_API UCombatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	void UpdateLookAtValues();
	void UpdateLeanAmount();
	void UpdateAimOffsetAlpha();

	void StoreCharacterInfo(const ACombatCharacter& InCharacterRef);

private:
	TWeakObjectPtr<UEquipmentComponent> WP_EquipmentComponent;
	TWeakObjectPtr<ACombatCharacter> WP_Character;

	EWeapon WeaponType;
	ECombat CombatType;

	float Speed;
	float Direction;
	float LocomotionRateScale;
	float LookAtPitch;
	float LookAtYaw;
	float TurnThreshold;
	float MinTurnLength;
	float AimOffsetAlpha;
	float AxisTurnValue;
	float MouseDeltaX;
	float BlockAlpha;
	float AimAlpha;

	bool bIsInAir;
	bool bIsInSlowMotion;
	bool IsTwoHandedWeaponEquipped;
	bool IsInCombat;
	bool IsLookingForward;
	bool IsShieldEquipped;
	void StoreCharacterInfo();
};