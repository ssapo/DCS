#pragma once

#include "CoreMinimal.h"
#include "Enumerations.generated.h"

UENUM()
enum class EState : uint8
{
	None = 0,
	Idle,
	Disabled,
	Attacking,
	Rolling,
	Falling,
	Interacting,
	Backstabbing,
	Parrying,
	Dead,
	Count,
};

UENUM()
enum class EItem : uint8
{
	None = 0				UMETA(DisplayName = "None"),
	Spell					UMETA(DisplayName = "Spell"),
	Shield					UMETA(DisplayName = "Shield"),
	Head					UMETA(DisplayName = "Head"),
	Top						UMETA(DisplayName = "Top"),
	Legs					UMETA(DisplayName = "Legs"),
	Hands					UMETA(DisplayName = "Hands"),
	Feet					UMETA(DisplayName = "Feet"),
	Arrows					UMETA(DisplayName = "Arrows"),
	Tool					UMETA(DisplayName = "Tool"),
	Material				UMETA(DisplayName = "Material"),
	Ring					UMETA(DisplayName = "Ring"),
	MeleeWeapon				UMETA(DisplayName = "MeleeWeapon"),
	RangeWeapon				UMETA(DisplayName = "RangeWeapon"),
	Necklace				UMETA(DisplayName = "Necklace"),
	Count					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeapon : uint8
{
	None					UMETA(DisplayName = "None"),
	Sword					UMETA(DisplayName = "Sword"),
	Axe						UMETA(DisplayName = "Axe"),
	Bow						UMETA(DisplayName = "Bow"),
	Spell					UMETA(DisplayName = "Spell"),
	Count					UMETA(Hidden),
};

UENUM()
enum class EStat : uint8
{
	None					UMETA(DisplayName = "None"),
	Health					UMETA(DisplayName = "Health"),
	Stamina					UMETA(DisplayName = "Stamina"),
	Mana					UMETA(DisplayName = "Mana"),
	Damage					UMETA(DisplayName = "Damage"),
	Armor					UMETA(DisplayName = "Armor"),
	CritChance				UMETA(DisplayName = "CritChance"),
	CritMultiplier			UMETA(DisplayName = "CritMultiplier"),
	AttackSpeed				UMETA(DisplayName = "AttackSpeed"),
	Block					UMETA(DisplayName = "Block"),
	MeleeAttackStaminaCost	UMETA(DisplayName = "MeleeAttackStaminaCost"),
	MagicDamage				UMETA(DisplayName = "MagicDamage"),
	CastingSpeed			UMETA(DisplayName = "CastingSpeed"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EEffect : uint8
{
	None					UMETA(DisplayName = "None"),
	Stun					UMETA(DisplayName = "Stun"),
	Knockdown				UMETA(DisplayName = "Knockdown"),
	Burning					UMETA(DisplayName = "Burning"),
	Backstab				UMETA(DisplayName = "Backstab"),
	Impact					UMETA(DisplayName = "Impact"),
	Parried					UMETA(DisplayName = "Parried"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EAIBehavior : uint8
{
	None					UMETA(DisplayName = "None"),
	Patrol					UMETA(DisplayName = "Patrol"),
	MeleeAttack				UMETA(DisplayName = "MeleeAttack"),
	RangeAttack				UMETA(DisplayName = "RangeAttack"),
	Approach				UMETA(DisplayName = "Approach"),
	Flee					UMETA(DisplayName = "Flee"),
	StrafeAround			UMETA(DisplayName = "StrafeAround"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EApplyEffectMethod : uint8
{
	None					UMETA(DisplayName = "None"),
	Stack					UMETA(DisplayName = "Stack"),
	Replace					UMETA(DisplayName = "Replace"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EAttackResult : uint8
{
	None					UMETA(DisplayName = "None"),
	Stack					UMETA(DisplayName = "Failed"),
	Replace					UMETA(DisplayName = "Replace"),
	Count					UMETA(Hidden)
};

UENUM()
enum class ECollisionPart : uint8
{
	None					UMETA(DisplayName = "None"),
	MainHandItem			UMETA(DisplayName = "MainHandItem"),
	RightHand				UMETA(DisplayName = "RightHand"),
	LeftHand				UMETA(DisplayName = "LeftHand"),
	RightFoot				UMETA(DisplayName = "RightFoot"),
	LeftFoot				UMETA(DisplayName = "LeftFoot"),
	Count					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECombat : uint8
{
	None					UMETA(DisplayName = "None"),
	Unarmed					UMETA(DisplayName = "Unarmed"),
	Melee					UMETA(DisplayName = "Melee"),
	Range					UMETA(DisplayName = "Range"),
	Magic					UMETA(DisplayName = "Magic"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EDirection : uint8
{
	None					UMETA(DisplayName = "None"),
	Foward					UMETA(DisplayName = "Foward"),
	Back					UMETA(DisplayName = "Back"),
	Left					UMETA(DisplayName = "Left"),
	Right					UMETA(DisplayName = "Right"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EMeleeAttack : uint8
{
	None					UMETA(DisplayName = "None"),
	Light					UMETA(DisplayName = "Light"),
	Heavy					UMETA(DisplayName = "Heavy"),
	Special					UMETA(DisplayName = "Special"),
	Thrust					UMETA(DisplayName = "Thrust"),
	Falling					UMETA(DisplayName = "Falling"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EInputBufferKey : uint8
{
	None					UMETA(DisplayName = "None"),
	LightAttack				UMETA(DisplayName = "LightAttack"),
	HeavyAttack				UMETA(DisplayName = "HeavyAttack"),
	ThrustAttack			UMETA(DisplayName = "ThrustAttack"),
	SpecialAttack			UMETA(DisplayName = "SpecialAttack"),
	FallingAttack			UMETA(DisplayName = "FallingAttack"),
	Roll					UMETA(DisplayName = "Roll"),
	Jump					UMETA(DisplayName = "Jump"),
	Parry					UMETA(DisplayName = "Parry"),
	ToggleCombat			UMETA(DisplayName = "ToggleCombat"),
	SwitchMainHandTypeUp	UMETA(DisplayName = "SwitchMainHandTypeUp"),
	SwitchMainHandTypeDown	UMETA(DisplayName = "SwitchMainHandTypeDown"),
	SwitchMainHandItemUp	UMETA(DisplayName = "SwitchMainHandItemUp"),
	SwitchMainHandItemDown	UMETA(DisplayName = "SwitchMainHandItemDown"),
	AbilityAttack			UMETA(DisplayName = "AbilityAttack"),
	SetSpellActiveIndex		UMETA(DisplayName = "SetSpellActiveIndex"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EHandleSameItemMethod : uint8
{
	None					UMETA(DisplayName = "None"),
	UnEquip					UMETA(DisplayName = "UnEquip"),
	Update					UMETA(DisplayName = "Update"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EMovementState : uint8
{
	None					UMETA(DisplayName = "None"),
	Idle					UMETA(DisplayName = "Idle"),
	Walk					UMETA(DisplayName = "Walk"),
	Jog						UMETA(DisplayName = "Jog"),
	Sprint					UMETA(DisplayName = "Sprint"),
	Count					UMETA(Hidden)
};

UENUM()
enum class ERotationMode : uint8
{
	None					UMETA(DisplayName = "None"),
	FaceCamera				UMETA(DisplayName = "FaceCamera"),
	OrientToMovement		UMETA(DisplayName = "OrientToMovement"),
	Count					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMontage : uint8
{
	None,
	LightAttack,
	HeavyAttack,
	ThrustAttack,
	SpecialAttack,
	FallingAttack,
	RollForward,
	RollLeft,
	RollRight,
	RollBackward,
	DrawWeapon,
	DisarmWeapon,
	StunFront,
	StunBack,
	StunRight,
	StunLeft,
	Impact,
	Block,
	Parry,
	Parried,
	Backstabbed,
	Backstab,
	ShootArrow,
	Count,
};

UENUM()
enum class EDemoRoom : uint8
{
	None					UMETA(DisplayName = "None"),
	Standard				UMETA(DisplayName = "Standard"),
	RoofWithHole			UMETA(DisplayName = "RoofWithHole"),
	RoofOpen				UMETA(DisplayName = "RoofOpen"),
	Count					UMETA(Hidden)
};

UENUM()
enum class EDemoDisplay : uint8
{
	Round					UMETA(DisplayName = "Round"),
	SquareL					UMETA(DisplayName = "SquareL"),
	RoomL					UMETA(DisplayName = "RoomL"),
	DescriptionOnly			UMETA(DisplayName = "DescriptionOnly"),
	SquareLFlatWall			UMETA(DisplayName = "SquareL_FlatWall"),
	Count					UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EActivity : uint8
{
	None,
	IsBlockingPressed,
	IsAimingPressed,
	IsImmortal,
	CantBeInterrupted,
	IsLoockingForward,
	CanParryHit,
	IsZooming,
	Count
};

UENUM()
enum class EWidgetID
{
	None = 0,
	InGame,
	KeyBindings,
};