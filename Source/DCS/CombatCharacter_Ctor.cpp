#include "CombatCharacter.h"

#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/CollisionHandlerComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/MontageManagerComponent.h"
#include "Components/EffectsComponent.h"
#include "Components/MovementSpeedComponent.h"
#include "Components/StatsManagerComponent.h"
#include "Components/DissolveComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/RotatingComponent.h"
#include "Components/DynamicTargetingComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Items/ObjectItems/ItemBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputBufferComponent.h"

ACombatCharacter::ACombatCharacter()
{
	CtorComponents();

	CtorInitialize();
}

void ACombatCharacter::CtorComponents()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	ensure(MeshComponent != nullptr);

	CCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	ensure(CCameraBoom != nullptr);
	CCameraBoom->SetupAttachment(RootComponent);

	CTargetingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TargetingArrow"));
	ensure(CTargetingArrow != nullptr);
	CTargetingArrow->SetupAttachment(RootComponent);

	CFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	ensure(CFollowCamera != nullptr);
	CFollowCamera->SetupAttachment(CCameraBoom);

	CEffectsAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EffectsAudio"));
	ensure(CEffectsAudio != nullptr);
	CEffectsAudio->SetupAttachment(MeshComponent);

	CArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSpawnLocation"));
	ensure(CArrowSpawnLocation != nullptr);
	CArrowSpawnLocation->SetupAttachment(MeshComponent);

	CStateManager = CreateDefaultSubobject<UStateManagerComponent>(TEXT("StateManager"));
	ensure(CStateManager != nullptr);

	CInputBuffer = CreateDefaultSubobject<UInputBufferComponent>(TEXT("InputBuffer"));
	ensure(CInputBuffer != nullptr);

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

	CMovementSpeed = CreateDefaultSubobject<UMovementSpeedComponent>(TEXT("MovementSpeed"));
	ensure(CMovementSpeed != nullptr);

	CStatsManager = CreateDefaultSubobject<UStatsManagerComponent>(TEXT("StatsManager"));
	ensure(CStatsManager != nullptr);

	CDissolve = CreateDefaultSubobject<UDissolveComponent>(TEXT("Dissolve"));
	ensure(CDissolve != nullptr);

	CInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	ensure(CInventory != nullptr);

	CEquipment = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
	ensure(CEquipment != nullptr);

	CRotating = CreateDefaultSubobject<URotatingComponent>(TEXT("Rotating"));
	ensure(CRotating != nullptr);

	CDynamicTargeting = CreateDefaultSubobject<UDynamicTargetingComponent>(TEXT("DynamicTargeting"));
	ensure(CDynamicTargeting != nullptr);
}

void ACombatCharacter::CtorInitialize()
{
	MeleeAttackType = EMeleeAttack::None;
	ReceivedHitDirection = EDirection::None;

	HorizontalLookRate = 45.0f;
	VerticalLookRate = 45.0f;
	RollStaminaCost = 25.0f;
	SprintStaminaCost = 0.5f;

	AimAlpha = 0.0f;

	SlowMotionTimeDuration = 0.4f;
	SlowMotionStaminaCost = 1.0f;
	ZoomCameraArmLength = 150.0f;

	CCameraBoom->TargetArmLength = InitialCameraArmLength;
	CCameraBoom->CameraLagSpeed = InitialCameraLagSpeed;

	PrimaryActorTick.bCanEverTick = true;
}