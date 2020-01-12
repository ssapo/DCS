// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"
#include "CombatCharacter.h"
#include "DCSLib.h"
#include "Components/EquipmentComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerController.h"

UCombatAnimInstance::UCombatAnimInstance()
{
	CombatType = ECombat::None;
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

	WP_Character->OnPostBeginPlay().AddUObject(this, &UCombatAnimInstance::BindDelegate);
	WP_Character->OnPostEndPlay().AddUObject(this, &UCombatAnimInstance::UnBindDelegate);
	
}

void UCombatAnimInstance::BindDelegate()
{
	check(WP_Character.IsValid());
	UEquipmentComponent* EquipComp = UDCSLib::GetComponent<UEquipmentComponent>(*WP_Character);
	check(EquipComp != nullptr);

	EquipComp->OnCombatChanged().AddWeakLambda(this,
		[=](bool IsCombat) { IsInCombat = IsCombat; });
	EquipComp->OnCombatTypeChanged().AddWeakLambda(this,
		[=](ECombat InType) { CombatType = InType; });
	EquipComp->OnMainHandTypeChanged().AddWeakLambda(this,
		[=](EItem InType) { UpdateHandItemsInfo(); });
	EquipComp->OnWeaponTypeChanged().AddWeakLambda(this,
		[=](EWeapon InType) { WeaponType = InType; });

	EquipComp->OnActiveItemChanged().AddUObject(this, &UCombatAnimInstance::OnActiveItemChanged);
}

void UCombatAnimInstance::UnBindDelegate()
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
	// TODO: Fill Function
}

void UCombatAnimInstance::UpdateLookAtValues()
{
	// TODO: Fill Function
}

void UCombatAnimInstance::UpdateLeanAmount()
{
	// TODO: Fill Function
}

void UCombatAnimInstance::UpdateAimOffsetAlpha()
{
	// TODO: Fill Function
}

void UCombatAnimInstance::UpdateHandItemsInfo()
{
	// TODO: Fill Function
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
