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
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		ECombat GetCombatType_BC() const { return CombatType; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		EWeapon GetWeaponType_BC() const { return WeaponType; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		float GetLeanAmount_BC() const { return LeanAmount; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		float GetLeanOffset_BC() const { return LeanOffset; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		float GetDirection_BC() const { return Direction; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		float GetSpeed_BC() const { return Speed; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		float GetLocomotionRateScale_BC() const { return LocomotionRateScale; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		float GetBlockAlpha_BC() const { return BlockAlpha; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		bool IsInCombat_BC() const { return IsInCombat; }

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
		bool IsShieldEquipped_BC() const { return IsShieldEquipped; }

private:
	void OnActiveItemChanged(const FStoredItem& Old, const FStoredItem& New, EItem InType, int32 SlotIndex, int32 ActiveIndex);

	void BindDelegate();
	void UnBindDelegate();

	void UpdateLookAtValues();
	void UpdateLeanAmount();
	void UpdateAimOffsetAlpha();
	void UpdateHandItemsInfo();

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
	float LeanAmount;
	float LeanOffset;

	bool bIsInAir;
	bool bIsInSlowMotion;
	bool IsTwoHandedWeaponEquipped;
	bool IsInCombat;
	bool IsLookingForward;
	bool IsShieldEquipped;
	void StoreCharacterInfo();
};
