#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "Animation/AnimInstance.h"
#include "WeakObjectPtrTemplates.h"
#include "CombatAnimInstance.generated.h"

class ACombatCharacter;
class UEquipmentComponent;
struct FStoredItem;

UCLASS()
class DCS_API UCombatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCombatAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	void OnCombatChanged(bool bInCombat);
	void OnCombatTypeChanged(ECombat InType);
	void OnMainHandTypeChanged(EItem InType);
	void OnWeaponTypeChanged(EWeapon InType);
	void OnActiveItemChanged(const FStoredItem& Old, const FStoredItem& New, EItem InType, int32 SlotIndex, int32 ActiveIndex);

	void PreBeginPlay();
	void PostEndPlay();

	void UpdateLookAtValues();
	void UpdateLeanAmount();
	void UpdateAimOffsetAlpha();
	void UpdateHandItemsInfo();

	void StoreCharacterInfo();

protected:
	UPROPERTY(Transient)
		TWeakObjectPtr<UEquipmentComponent> WP_CEquip;

	UPROPERTY(Transient)
		TWeakObjectPtr<ACombatCharacter> WP_Character;

	UPROPERTY(Transient, BlueprintReadOnly)
		EWeapon WeaponType;

	UPROPERTY(Transient, BlueprintReadOnly)
		ECombat CombatType;

	UPROPERTY(Transient, BlueprintReadOnly)
		float Speed;

	UPROPERTY(Transient, BlueprintReadOnly)
		float Direction;

	UPROPERTY(Transient, BlueprintReadOnly)
		float LocomotionRateScale;

	UPROPERTY(Transient, BlueprintReadOnly)
		float LookAtPitch;

	UPROPERTY(Transient, BlueprintReadOnly)
		float LookAtYaw;

	UPROPERTY(Transient, BlueprintReadOnly)
		float TurnThreshold;

	UPROPERTY(Transient, BlueprintReadOnly)
		float MinTurnLength;

	UPROPERTY(Transient, BlueprintReadOnly)
		float AimOffsetAlpha;

	UPROPERTY(Transient, BlueprintReadOnly)
		float AxisTurnValue;

	UPROPERTY(Transient, BlueprintReadOnly)
		float MouseDeltaX;

	UPROPERTY(Transient, BlueprintReadOnly)
		float MouseDeltaY;

	UPROPERTY(Transient, BlueprintReadOnly)
		float BlockAlpha;

	UPROPERTY(Transient, BlueprintReadOnly)
		float AimAlpha;

	UPROPERTY(Transient, BlueprintReadOnly)
		float LeanAmount;

	UPROPERTY(Transient, BlueprintReadOnly)
		float LeanOffset;

	UPROPERTY(Transient, BlueprintReadOnly)
		bool bIsInAir;

	UPROPERTY(Transient, BlueprintReadOnly)
		bool bIsInSlowMotion;

	UPROPERTY(Transient, BlueprintReadOnly)
		bool IsTwoHandedWeaponEquipped;

	UPROPERTY(Transient, BlueprintReadOnly)
		bool IsInCombat;

	UPROPERTY(Transient, BlueprintReadOnly)
		bool IsLookingForward;

	UPROPERTY(Transient, BlueprintReadOnly)
		bool IsShieldEquipped;
};
