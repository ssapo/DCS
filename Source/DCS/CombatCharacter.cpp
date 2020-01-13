// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
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
#include "Camera/CameraComponent.h"

#include "UserWidget.h"
#include "Defines.h"
#include "WidgetSystem.h"
#include "DataTables.h"
#include "Widgets/KeybindingsWidget.h"
#include "Widgets/InGameWidget.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DCSLib.h"
#include "DCS.h"

// start public:
ACombatCharacter::ACombatCharacter()
{
	CtorComponents();

	CtorInitialize();

	PrimaryActorTick.bCanEverTick = true;
}

FORCEINLINE float ACombatCharacter::GetJogSpeed() const
{
	return CMovementSpeed->GetJogSpeed();
}

FORCEINLINE bool ACombatCharacter::IsInSlowMotion() const
{
	return bIsInSlowMotion;
}

FORCEINLINE float ACombatCharacter::GetBlockAlpha() const
{
	return BlockAlpha;
}

FORCEINLINE float ACombatCharacter::GetAimAlpha() const
{
	return AimAlpha;
}

FORCEINLINE bool ACombatCharacter::IsActivityPure(EActivity InType) const
{
	return CStateManager->GetActivityValue(InType);
}

FORCEINLINE bool ACombatCharacter::IsIdleAndNotFalling() const
{
	return GetCharacterMovement()->IsFalling() && IsStateEqualPure(EState::Idle);
}

FORCEINLINE bool ACombatCharacter::IsStateEqualPure(EState InType) const
{
	return CStateManager->GetState() == InType;
}

UDataTable* ACombatCharacter::GetMontages(EMontage InType) const
{
	return nullptr;
}

// end public:

// start private:
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

	HorizontalLockRate = 45.0f;
	VerticalLookRate = 45.0f;
	RollStaminaCost = 25.0f;
	SprintStaminaCost = 0.5f;

	AimAlpha = 0.0f;

	SlowMotionTimeDuration = 0.4f;
	SlowMotionStaminaCost = 1.0f;
	ZoomCameraArmLength = 150.0f;

	CCameraBoom->TargetArmLength = InitialCameraArmLength;
	CCameraBoom->CameraLagSpeed = InitialCameraLagSpeed;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponents();

	CreateHUD();

	SetTimerChecker();

	check(CInputBuffer);
	CInputBuffer->OnInputBufferConsumed().AddUObject(this, &ACombatCharacter::OnInputBufferConsumed);

	PostBeginPlayEvent.Broadcast();
	PostBeginPlayEvent.Clear();
}

void ACombatCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	check(CInputBuffer);
	CInputBuffer->OnInputBufferConsumed().RemoveAll(this);

	PostEndPlayEvent.Broadcast();
	PostEndPlayEvent.Clear();
}

void ACombatCharacter::InitializeComponents()
{
	CDynamicTargeting->Initialize(*CTargetingArrow);
	CStatsManager->Initialize();
	CEquipment->Initialize();
}

void ACombatCharacter::CreateHUD()
{
	CreateInGameWidget();

	CreateKeyBindings();
}

void ACombatCharacter::SetTimerChecker()
{
	FTimerManager& TimerMangaer = GetWorld()->GetTimerManager();

	// TODO: fill function
	auto CheckForInteractable = [this]() {
		FVector Start = GetActorLocation();
		FVector End = GetActorForwardVector() * 100.0f + Start;

		float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		const TArray<TEnumAsByte<EObjectTypeQuery>> Types{ EObjectTypeQuery::ObjectTypeQuery1 };
		const TArray<AActor*> Ignores;
		EDrawDebugTrace::Type Debug = EDrawDebugTrace::None;

		FHitResult Result;
		if (UDCSLib::CapsuleTraceForObjects(this, Start, End, 20.0f, HalfHeight, Types, false, Ignores, Debug, Result, true))
		{
			auto WPActor = Result.Actor;
			if (WPActor.IsValid() && WP_InteractionActor != WPActor)
			{
				WP_InteractionActor = WPActor;

				//WP_InGameWidget->WBinter

				WP_InGameWidget->UpdateWidget();
			}
		}
		else
		{
			WP_InGameWidget->UpdateWidget();
		}
		
	};

	TimerMangaer.SetTimer(CheckTimer, CheckForInteractable, 0.1f, true);
}

void ACombatCharacter::OnRollKeyPressed()
{
	CInputBuffer->UpdateKey(EInputBufferKey::Roll);
}

void ACombatCharacter::OnJumpKeyPressed()
{
	CInputBuffer->UpdateKey(EInputBufferKey::Jump);
}

void ACombatCharacter::OnJumpKeyReleased()
{
	CInputBuffer->StopJumping();
}

void ACombatCharacter::OnToggleCombatKeyPressed()
{
	if (IsIdleAndNotFalling() && IsActivityPure(EActivity::IsAimingPressed))
	{
		ResetAimingMode();
	}
	else
	{
		DLOG_S(Log);
		CInputBuffer->UpdateKey(EInputBufferKey::ToggleCombat);
	}
}

void ACombatCharacter::OnToggleKeyPressed()
{
	CMovementSpeed->ToggleState();
}

void ACombatCharacter::OnSprintKeyPressed()
{
	SetSprint(true);
}

void ACombatCharacter::OnSprintKeyReleased()
{
	SetSprint(false);
}

void ACombatCharacter::OnInputBufferConsumed(EInputBufferKey InKey)
{
	if (IsAlive() == false)
	{
		return;
	}

	switch (InKey)
	{	
	default:
		return;

	case EInputBufferKey::LightAttack:
		break;
	case EInputBufferKey::HeavyAttack:
		break;
	case EInputBufferKey::ThrustAttack:
		break;
	case EInputBufferKey::SpecialAttack:
		break;
	case EInputBufferKey::FallingAttack:
		break;
	case EInputBufferKey::Roll:
		break;
	case EInputBufferKey::Jump:
		break;
	case EInputBufferKey::Parry:
		break;
	case EInputBufferKey::ToggleCombat:
		ToggleCombat();
		break;
	case EInputBufferKey::SwitchMainHandTypeUp:
		break;
	case EInputBufferKey::SwitchMainHandTypeDown:
		break;
	case EInputBufferKey::SwitchMainHandItemUp:
		break;
	case EInputBufferKey::SwitchMainHandItemDown:
		break;
	}
}

void ACombatCharacter::OnMoveForward(float InAxisValue)
{
	if (IsAlive())
	{
		FRotator Rot = FRotator(0.0f, 0.0f, GetControlRotation().Yaw);
		AddMovementInput(UDCSLib::GetForwardVector(Rot), InAxisValue);
	}
}

void ACombatCharacter::OnMoveRight(float InAxisValue)
{
	if (IsAlive())
	{
		FRotator Rot = FRotator(0.0f, 0.0f, GetControlRotation().Yaw);
		AddMovementInput(UDCSLib::GetRightVector(Rot), InAxisValue);
	}
}

void ACombatCharacter::OnHorizontalLook(float InAxisValue)
{
	AddControllerYawInput(HorizontalLockRate * InAxisValue * UDCSLib::GetDTS(this));
	CDynamicTargeting->FindTargetWithAxisInput(InAxisValue);
}

void ACombatCharacter::OnVerticalLook(float InAxisValue)
{
	AddControllerPitchInput(VerticalLookRate * InAxisValue * UDCSLib::GetDTS(this));
}

void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAimAlpha();
}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent != nullptr);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(EVENT_KEYBIND, IE_Pressed, this, 
		&ACombatCharacter::OnShowKeyBindings);
	PlayerInputComponent->BindAction(EVENT_KEYBIND, IE_Released, this, 
		&ACombatCharacter::OnHideKeyBindings);

	PlayerInputComponent->BindAxis(EVENT_MOVEFORWARD, this,
		&ACombatCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis(EVENT_MOVERIGHT, this,
		&ACombatCharacter::OnMoveRight);

	PlayerInputComponent->BindAction(EVENT_JUMP, IE_Pressed, this, 
		&ACombatCharacter::OnJumpKeyPressed);
	PlayerInputComponent->BindAction(EVENT_JUMP, IE_Released, this,
		&ACombatCharacter::OnJumpKeyReleased);
	PlayerInputComponent->BindAction(EVENT_ROLL, IE_Pressed, this,
		&ACombatCharacter::OnRollKeyPressed);

	PlayerInputComponent->BindAction(EVENT_TOGGLECOMBAT, IE_Pressed, this,
		&ACombatCharacter::OnToggleCombatKeyPressed);
}

// start interfaces
void ACombatCharacter::Interact(AActor* Actor)
{
	// TODO: fill function
}

void ACombatCharacter::OpenedUI()
{
	// TODO: fill function
}

void ACombatCharacter::ClosedUI()
{
	// TODO: fill function
}

EAttackResult ACombatCharacter::TakeDamage(const FHitData& HitData)
{
	// TODO: fill function
	return EAttackResult::None;
}

FName ACombatCharacter::GetInteractionMessage() const
{
	// TODO: fill function
	return FName();
}

FName ACombatCharacter::GetHeadSocket() const
{
	// TODO: fill function
	return FName();
}

FName ACombatCharacter::GetBowStringSocketName() const
{
	// TODO: fill function
	return FName();
}

FRotator ACombatCharacter::GetDesiredRotation() const
{
	// TODO: fill function
	return FRotator();
}

bool ACombatCharacter::IsAlive() const
{
	// TODO: fill function
	return true;
}

bool ACombatCharacter::CanEffectBeApplied(EEffect InType, AActor* Actor) const
{
	// TODO: fill function
	return true;
}

bool ACombatCharacter::DoesHoldBowString()
{
	return true;
}
// end interfaces

void ACombatCharacter::CreateKeyBindings()
{
	
	if (auto NewWidget = Cast<UKeybindingsWidget>(ShowWidget(EWidgetID::KeyBindings)))
	{
		WP_KeyBindingsWidget = NewWidget;
	}
}

void ACombatCharacter::CreateInGameWidget()
{
	if (auto NewWidget = Cast<UInGameWidget>(ShowWidget(EWidgetID::InGame)))
	{
		WP_InGameWidget = NewWidget;
	}
}

void ACombatCharacter::UpdateAimAlpha()
{
	// TODO: fill function
}

void ACombatCharacter::OnShowKeyBindings()
{
	if (WP_KeyBindingsWidget.IsValid())
	{
		WP_KeyBindingsWidget->ShowKeyBindings();
	}
}

void ACombatCharacter::OnHideKeyBindings()
{
	if (WP_KeyBindingsWidget.IsValid())
	{
		WP_KeyBindingsWidget->HideKeyBindings();
	}
}

void ACombatCharacter::SetSprint(bool bActivate)
{
	// TODO: fill function
}

void ACombatCharacter::ResetAimingMode()
{
	StopLookingForward();

	StopAming();

	StopZooming();

	HideCrossHair();
}

void ACombatCharacter::StopLookingForward()
{
	// TODO: fill function
}

void ACombatCharacter::StopAming()
{
	// TODO: fill function
}

void ACombatCharacter::StopZooming()
{
	// TODO: fill function
}

void ACombatCharacter::HideCrossHair()
{
	// TODO: fill function
}

void ACombatCharacter::ToggleCombat()
{
	if (IsStateEqualPure(EState::Idle) == false)
	{
		return;
	}

	CStateManager->SetState(EState::Interacting);
	EMontage Action = CEquipment->IsInCombat() ? EMontage::DrawWeapon : EMontage::DisarmWeapon;
	if (UAnimMontage* Animation = CMontagesManager->GetMontageForAction(Action, 0))
	{
		PlayAnimMontage(Animation);
	}
	else
	{
		CEquipment->ToggleCombat();
		CStateManager->ResetState();
	}
}

FORCEINLINE UDCSWidget* ACombatCharacter::ShowWidget(EWidgetID InType) const
{
	return UWidgetSystem::Get()->ShowWidget(InType);
}

