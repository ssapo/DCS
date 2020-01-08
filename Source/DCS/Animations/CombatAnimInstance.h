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
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UFUNCTION(BlueprintPure, BlueprintThreadSafe)
		ECombat GetCombatType_BC() const { return CombatType; }

	UFUNCTION(BlueprintPure, BlueprintThreadSafe)
		float GetLeanAmount_BC() const { return LeanAmount; }

	UFUNCTION(BlueprintPure, BlueprintThreadSafe)
		float GetLeanOffset_BC() const { return LeanOffset; }

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
