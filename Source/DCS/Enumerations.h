#pragma once

#include "CoreMinimal.h"
#include "Enumerations.generated.h"

UENUM()
enum class EState : int8
{
	None = 0			UMETA(DisplayName = "None"),
	Disabled			UMETA(DisplayName = "Disabled"),
	Attacking			UMETA(DisplayName = "Attacking"),
	Rolling				UMETA(DisplayName = "Rolling"),
	Falling				UMETA(DisplayName = "Falling"),
	Interacting			UMETA(DisplayName = "Interacting"),
	Backstabbing		UMETA(DisplayName = "Backstabbing"),
	Parrying			UMETA(DisplayName = "Parrying"),
	Dead				UMETA(DisplayName = "Dead"),
	Count				UMETA(Hidden)
};

UENUM()
enum class EItem : int8
{
	None = 0			UMETA(DisplayName = "None"),
	Spell				UMETA(DisplayName = "Spell"),
	Shield				UMETA(DisplayName = "Shield"),
	Head				UMETA(DisplayName = "Head"),
	Top					UMETA(DisplayName = "Top"),
	Legs				UMETA(DisplayName = "Legs"),
	Hands				UMETA(DisplayName = "Hands"),
	Feet				UMETA(DisplayName = "Feet"),
	Arrows				UMETA(DisplayName = "Arrows"),
	Tool				UMETA(DisplayName = "Tool"),
	Material			UMETA(DisplayName = "Material"),
	Ring				UMETA(DisplayName = "Ring"),
	MeleeWeapon			UMETA(DisplayName = "MeleeWeapon"),
	RangeWeapon			UMETA(DisplayName = "RangeWeapon"),
	Necklace			UMETA(DisplayName = "Necklace"),
	Count				UMETA(Hidden)
};

UENUM()
enum class EWeapon : int8
{
	None,
	Count
};

UENUM()
enum class EStat : int8
{
	None,
	Count
};

UENUM()
enum class EEffect : int8
{
	None				UMETA(DisplayName = "None"),
	Stun				UMETA(DisplayName = "Stun"),
	Knockdown			UMETA(DisplayName = "Knockdown"),
	Burning				UMETA(DisplayName = "Burning"),
	Backstab			UMETA(DisplayName = "Backstab"),
	Impact				UMETA(DisplayName = "Impact"),
	Parried				UMETA(DisplayName = "Parried"),
	Count				UMETA(Hidden)
};

UENUM()
enum class EAIBehavior : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EApplyEffectMethod : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EAttackResult : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class ECollisionPart : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class ECombat : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EDirection : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EMeleeAttack : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EInputBufferKey : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EHandleSameItemMethod : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EMovementState : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class ERotationMode : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EMontage : int8
{
	None,
	Count				UMETA(Hidden)
};

UENUM()
enum class EDemoRoom : int8
{
	None				UMETA(DisplayName = "None"),
	Standard			UMETA(DisplayName = "Standard"),
	RoofWithHole		UMETA(DisplayName = "RoofWithHole"),
	RoofOpen			UMETA(DisplayName = "RoofOpen"),
	Count				UMETA(Hidden)
};

UENUM()
enum class EDemoDisplay : int8
{
	Round				UMETA(DisplayName = "Round"),
	SquareL				UMETA(DisplayName = "SquareL"),
	RoomL				UMETA(DisplayName = "RoomL"),
	DescriptionOnly		UMETA(DisplayName = "DescriptionOnly"),
	SquareLFlatWall		UMETA(DisplayName = "SquareL_FlatWall"),
	Count				UMETA(Hidden)
};