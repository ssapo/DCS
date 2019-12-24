// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"

void UCombatAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
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

void UCombatAnimInstance::UpdateLookAtValues()
{

}

void UCombatAnimInstance::UpdateLeanAmount()
{

}

void UCombatAnimInstance::UpdateAimOffsetAlpha()
{

}

void UCombatAnimInstance::StoreCharacterInfo(const ACombatCharacter& InCharacterRef)
{
	
}
