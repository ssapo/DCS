// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"
#include "CombatCharacter.h"
#include "DCSLib.h"
#include "Components/EquipmentComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Items/ObjectItems/ItemBase.h"

UCombatAnimInstance::UCombatAnimInstance()
{
	CombatType = ECombat::Unarmed;
	WeaponType = EWeapon::None;

	LocomotionRateScale = 1.0f;
	BlockAlpha = 0.0f;
	AimAlpha = 1.0f;
	Speed = 0.0f;

	LeanAmount = 0.0f;
	LeanOffset = 10.0f;

	MouseDeltaX = 0.0f;
	MouseDeltaY = 0.0f;

	IsInCombat = false;
	IsShieldEquipped = false;
}

void UCombatAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	WP_Character = Cast<ACombatCharacter>(TryGetPawnOwner());
	check(WP_Character.IsValid());

	WP_Character->OnPreBeginPlay().AddUObject(this, &UCombatAnimInstance::PreBeginPlay);
	WP_Character->OnPostEndPlay().AddUObject(this, &UCombatAnimInstance::PostEndPlay);
}

void UCombatAnimInstance::PreBeginPlay()
{
	check(WP_Character.IsValid());
	WP_CEquip = UDCSLib::GetComponent<UEquipmentComponent>(*WP_Character);
	check(WP_CEquip.IsValid());

	WP_CEquip->OnCombatChanged().AddUObject(this, &UCombatAnimInstance::OnCombatChanged);
	WP_CEquip->OnCombatTypeChanged().AddUObject(this, &UCombatAnimInstance::OnCombatTypeChanged);
	WP_CEquip->OnMainHandTypeChanged().AddUObject(this, &UCombatAnimInstance::OnMainHandTypeChanged);
	WP_CEquip->OnWeaponTypeChanged().AddUObject(this, &UCombatAnimInstance::OnWeaponTypeChanged);
	WP_CEquip->OnActiveItemChanged().AddUObject(this, &UCombatAnimInstance::OnActiveItemChanged);
}

void UCombatAnimInstance::PostEndPlay()
{
	check(WP_Character.IsValid());
	UEquipmentComponent* EquipComp = UDCSLib::GetComponent<UEquipmentComponent>(*WP_Character);
	check(EquipComp != nullptr);

	EquipComp->OnCombatChanged().RemoveAll(this);
	EquipComp->OnCombatTypeChanged().RemoveAll(this);
	EquipComp->OnMainHandTypeChanged().RemoveAll(this);
	EquipComp->OnWeaponTypeChanged().RemoveAll(this);
	EquipComp->OnActiveItemChanged().RemoveAll(this);
}

void UCombatAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (WP_Character.IsValid() == false)
	{
		return;
	}

	StoreCharacterInfo();

	UpdateLeanAmount();

	UpdateLookAtValues();

	UpdateAimOffsetAlpha();
}

void UCombatAnimInstance::OnActiveItemChanged(const FStoredItem& Old, const FStoredItem& New, EItem InType, int32 SlotIndex, int32 ActiveIndex)
{
	UpdateHandItemsInfo();
}

void UCombatAnimInstance::UpdateLookAtValues()
{
	FRotator CurrentRot = UDCSLib::MakeRot(0.0f, LookAtPitch, LookAtYaw);
	FRotator TargetRot = UDCSLib::Delta(WP_Character->GetControlRotation(), WP_Character->GetActorRotation());
	FRotator Constant = UDCSLib::InterpTo(CurrentRot, TargetRot, UDCSLib::GetDTS(this), 15.0f);
	LookAtPitch = FMath::ClampAngle(Constant.Pitch, -90.0f, 90.0f);
	LookAtYaw = FMath::ClampAngle(Constant.Yaw, -90.0f, 90.0f);
}

void UCombatAnimInstance::UpdateLeanAmount()
{
	const TTuple<float, float>& CharacterLeanAmount = WP_Character->CalculateLeanAmount();
	LeanAmount = FMath::FInterpTo(LeanAmount, CharacterLeanAmount.Key, UDCSLib::GetDTS(this), CharacterLeanAmount.Value);
}

void UCombatAnimInstance::UpdateAimOffsetAlpha()
{
	AimOffsetAlpha = UDCSLib::InterpTo(AimOffsetAlpha, IsLookingForward, UDCSLib::GetDTS(this), 5.0f);
}

void UCombatAnimInstance::UpdateHandItemsInfo()
{
	IsShieldEquipped = WP_CEquip->IsShieldEquipped();
	IsTwoHandedWeaponEquipped = WP_CEquip->IsTwoHandedWeaponEquipped();
}

void UCombatAnimInstance::StoreCharacterInfo()
{
	bIsInAir = WP_Character->GetMovementComponent()->IsFalling();

	FVector Velocity = WP_Character->GetVelocity();
	FRotator Rotation = WP_Character->GetActorRotation();

	Speed = Velocity.Size();
	Direction = CalculateDirection(Velocity, Rotation);

	float JogSpeed = WP_Character->GetJogSpeed();
	float Select = 0.0f;
	if (Speed <= JogSpeed)
	{
		Select = 1.0f;
	}
	else
	{
		Select = Speed / JogSpeed;
	}
	LocomotionRateScale = Select;

	bIsInSlowMotion = WP_Character->IsInSlowMotion();
	BlockAlpha = WP_Character->GetBlockAlpha();
	AimAlpha = WP_Character->GetAimAlpha();

	if (APlayerController* PC = UDCSLib::GetPlayerController(this))
	{
		PC->GetInputMouseDelta(MouseDeltaX, MouseDeltaY);
	}
}

void UCombatAnimInstance::OnCombatChanged(bool bInCombat)
{
	IsInCombat = bInCombat;
}

void UCombatAnimInstance::OnCombatTypeChanged(ECombat InType)
{
	CombatType = InType;
}

void UCombatAnimInstance::OnMainHandTypeChanged(EItem InType)
{
	UpdateHandItemsInfo();
}

void UCombatAnimInstance::OnWeaponTypeChanged(EWeapon InType)
{
	WeaponType = InType;
}
