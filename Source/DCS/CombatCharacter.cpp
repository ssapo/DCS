// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/EffectsComponent.h"
#include "Components/RotatingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/DissolveComponent.h"
#include "Components/MovementSpeedComponent.h"
#include "Components/StatsManagerComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/MontageManagerComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/CollisionHandlerComponent.h"
#include "Components/InputBufferComponent.h"
#include "Components/DynamicTargetingComponent.h"

#include "UserWidget.h"
#include "Widgets/KeybindingsWidget.h"
#include "Define.h"

ACombatCharacter::ACombatCharacter()
{
	CtorComponents();

	CtorInitialize();

	PrimaryActorTick.bCanEverTick = true;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatCharacter::CtorComponents()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	ensure(MeshComponent != nullptr);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	ensure(CameraBoom.IsValid() == true);
	CameraBoom->SetupAttachment(RootComponent);

	TargetingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TargetingArrow"));
	ensure(TargetingArrow.IsValid() == true);
	TargetingArrow->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	ensure(FollowCamera.IsValid() == true);
	FollowCamera->SetupAttachment(CameraBoom.Get());

	EffectsAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EffectsAudio"));
	ensure(EffectsAudio.IsValid() == true);
	EffectsAudio->SetupAttachment(MeshComponent);

	ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSpawnLocation"));
	ensure(ArrowSpawnLocation.IsValid() == true);
	ArrowSpawnLocation->SetupAttachment(MeshComponent);

	StateManager = CreateDefaultSubobject<UStateManagerComponent>(TEXT("StateManager"));
	ensure(StateManager.IsValid() == true);

	InputBuffer = CreateDefaultSubobject<UInputBufferComponent>(TEXT("InputBuffer"));
	ensure(InputBuffer.IsValid() == true);

	MeleeCollisionHandler = CreateDefaultSubobject<UCollisionHandlerComponent>(TEXT("MeleeCollisionHandler"));
	ensure(MeleeCollisionHandler.IsValid() == true);

	ExtendedHealth = CreateDefaultSubobject<UExtendedStatComponent>(TEXT("ExtendedHealth"));
	ensure(ExtendedHealth.IsValid() == true);

	ExtendedStamina = CreateDefaultSubobject<UExtendedStatComponent>(TEXT("ExtendedStamina"));
	ensure(ExtendedStamina.IsValid() == true);

	MontagesManager = CreateDefaultSubobject<UMontageManagerComponent>(TEXT("MontagesManager"));
	ensure(MontagesManager.IsValid() == true);

	Effects = CreateDefaultSubobject<UEffectsComponent>(TEXT("Effects"));
	ensure(Effects.IsValid() == true);

	MovementSpeed = CreateDefaultSubobject<UMovementSpeedComponent>(TEXT("MovementSpeed"));
	ensure(MovementSpeed.IsValid() == true);

	StatsManager = CreateDefaultSubobject<UStatsManagerComponent>(TEXT("StatsManager"));
	ensure(StatsManager.IsValid() == true);

	Dissolve = CreateDefaultSubobject<UDissolveComponent>(TEXT("Dissolve"));
	ensure(Dissolve.IsValid() == true);

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	ensure(Inventory.IsValid() == true);

	Equipment = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
	ensure(Equipment.IsValid() == true);

	Rotating = CreateDefaultSubobject<URotatingComponent>(TEXT("Rotating"));
	ensure(Rotating.IsValid() == true);
}

void ACombatCharacter::CtorInitialize()
{
	MeleeAttackType = EMeleeAttack::None;
	ReceivedHitDirection = EDirection::None;

	HorizontalLockRate = 45.0f;
	VerticalLookRate = 45.0f;
	RollStaminaCost = 25.0f;
	SprintStaminaCost = 0.5f;

	SlowMotionTimeDuration = 0.4f;
	SlowMotionStaminaCost = 1.0f;
	ZoomCameraArmLength = 150.0f;

	CameraBoom->TargetArmLength = InitialCameraArmLength;
	CameraBoom->CameraLagSpeed = InitialCameraLagSpeed;
}

void ACombatCharacter::ShowKeyBindings()
{
	UE_LOG(LogTemp, Log, TEXT("ShowKeyBindings"));
}

void ACombatCharacter::HideKeyBindings()
{
	UE_LOG(LogTemp, Log, TEXT("HideKeyBindings"));
}

void ACombatCharacter::UpdateAimAlpha()
{
}

void ACombatCharacter::CreateKeyBindings()
{
	auto Player = GetController<APlayerController>();
	ensure(Player != nullptr);

	if (auto NewWidget = CreateWidget<UKeybindingsWidget>(Player))
	{
		NewWidget->AddToViewport();
	}
}

void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAimAlpha();
}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(EVENT_KEYBIND, IE_Pressed, this, &ACombatCharacter::ShowKeyBindings);
	PlayerInputComponent->BindAction(EVENT_KEYBIND, IE_Released, this, &ACombatCharacter::HideKeyBindings);
}

