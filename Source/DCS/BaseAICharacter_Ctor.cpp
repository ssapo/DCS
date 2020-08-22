#include "BaseAICharacter.h"

#include "Components/CollisionHandlerComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/MontageManagerComponent.h"
#include "Components/EffectsComponent.h"
#include "Components/DissolveComponent.h"
#include "Components/MovementSpeedComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UMG/Public/Components/WidgetComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	CtorComponents();

	CtorInitialize();
}

void ABaseAICharacter::CtorComponents()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	ensure(MeshComponent != nullptr);
	
	CMeleeCollisionHandler = CreateDefaultSubobject<UCollisionHandlerComponent>(TEXT("MeleeCollisionHandler"));
	ensure(CMeleeCollisionHandler != nullptr);
	
	CExtendedHealth = CreateDefaultSubobject<UExtendedStatComponent>(TEXT("ExtendedHealth"));
	ensure(CExtendedHealth != nullptr);

	CExtendedStamina = CreateDefaultSubobject<UExtendedStatComponent>(TEXT("ExtendedStamina"));
	ensure(CExtendedStamina != nullptr);

	CMontagesManager = CreateDefaultSubobject<UMontageManagerComponent>(TEXT("MontagesManager"));
	ensure(CMontagesManager != nullptr);
	
	CEffects = CreateDefaultSubobject<UEffectsComponent>(TEXT("Effects"));
	ensure(CEffects != nullptr);

	CDissolveEffect = CreateDefaultSubobject<UDissolveComponent>(TEXT("DissolveEffect"));
	ensure(CDissolveEffect != nullptr);

	CEquipment = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
	ensure(CEquipment != nullptr);

	CRotating = CreateDefaultSubobject<URotatingComponent>(TEXT("Rotating"));
	ensure(CRotating != nullptr);

	CStateManager = CreateDefaultSubobject<UStateManagerComponent>(TEXT("StateManager"));
	ensure(CStateManager != nullptr);

	CPatrol = CreateDefaultSubobject<UPatrolComponent>(TEXT("Patrol"));
	ensure(CPatrol != nullptr);

	CMovementSpeed = CreateDefaultSubobject<UMovementSpeedComponent>(TEXT("MovementSpeed"));
	ensure(CMovementSpeed != nullptr);

	CStatBarsWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatBarsWidget"));
	ensure(CStatBarsWidget != nullptr);

	CTargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
	ensure(CTargetWidget != nullptr);
}

void ABaseAICharacter::CtorInitialize()
{
	MeleeAttackType = EMeleeAttack::None;
	ReceivedHitDirection = EDirection::None;
}

