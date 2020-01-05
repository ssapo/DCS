// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"
#include "CombatCharacter.h"
#include "DCSLib.h"
#include "Components/EquipmentComponent.h"


void UCombatAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	WP_Character = Cast<ACombatCharacter>(TryGetPawnOwner());
	check(WP_Character.IsValid());

	WP_Character->OnPostBeginPlay().AddUObject(this, &UCombatAnimInstance::BindDelegate);
	WP_Character->OnPostEndPlay().AddUObject(this, &UCombatAnimInstance::UnBindDelegate);

	//BindDelegate();
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

	if (WP_Character.IsValid())
	{
		StoreCharacterInfo(*WP_Character);
	}

	UpdateLeanAmount();

	UpdateLookAtValues();

	UpdateAimOffsetAlpha();
}

void UCombatAnimInstance::OnActiveItemChanged(const FStoredItem& Old, const FStoredItem& New, EItem InType, int32 SlotIndex, int32 ActiveIndex)
{
	
}

void UCombatAnimInstance::UpdateLookAtValues()
{

}

void UCombatAnimInstance::UpdateLeanAmount()
{

}

void UCombatAnimInstance::UpdateAimOffsetAlpha()
{

}

void UCombatAnimInstance::UpdateHandItemsInfo()
{

}

void UCombatAnimInstance::StoreCharacterInfo(const ACombatCharacter& InCharacterRef)
{
	
}
