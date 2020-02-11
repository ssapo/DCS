// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/Actor.h"
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

#include "Widgets/KeybindingsWidget.h"
#include "Widgets/InGameWidget.h"

#include "UserWidget.h"
#include "Defines.h"
#include "WidgetSystem.h"
#include "DataTables.h"
#include "TimerManager.h"
#include "DCSLib.h"
#include "DCS.h"
#include "Engine/World.h"

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
	return !GetCharacterMovement()->IsFalling() && IsStateEqual(EState::Idle);
}

FORCEINLINE bool ACombatCharacter::IsStateEqual(EState InType) const
{
	return CStateManager->GetState() == InType;
}

FORCEINLINE bool ACombatCharacter::IsCombatEqual(ECombat InType) const
{
	return CEquipment->GetCombatType() == InType;
}

const TTuple<float, float> ACombatCharacter::CalculateLeanAmount() const
{
	TTuple<float, float> ReturnValue;

	EMovementState StateType = CMovementSpeed->GetMovementState();
	bool IdleAndNotFalling = IsIdleAndNotFalling();
	bool JogOrSprint = (StateType == EMovementState::Jog || StateType == EMovementState::Sprint);
	bool FasterThan10 = (GetCharacterMovement()->Velocity.Size() > 10.0f);
	bool LookingFowardIsDisabled = (IsActivityPure(EActivity::IsLoockingForward) == false);

	if (IdleAndNotFalling && JogOrSprint && FasterThan10 && LookingFowardIsDisabled)
	{
		ReturnValue.Key = FMath::Clamp(GetInputAxisValue(EVENT_LOOKH), -1.0f, 1.0f);
		ReturnValue.Value = 1.0f;
	}
	else
	{
		ReturnValue.Key = 0.0f;
		ReturnValue.Value = 10.0f;
	}

	return ReturnValue;
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
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponents();

	CreateHUD();

	SetTimerChecker();

	CInputBuffer->OnInputBufferClosed().AddUObject(this, &ACombatCharacter::OnInputBufferClosed);
	CInputBuffer->OnInputBufferConsumed().AddUObject(this, &ACombatCharacter::OnInputBufferConsumed);
	CRotating->OnRotatingStart().AddUObject(this, &ACombatCharacter::OnRotatingStart);
	CRotating->OnRotatingEnd().AddUObject(this, &ACombatCharacter::OnRotatingEnd);

	PostBeginPlayEvent.Broadcast();
	PostBeginPlayEvent.Clear();
}

void ACombatCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	CInputBuffer->OnInputBufferConsumed().RemoveAll(this);
	CRotating->OnRotatingStart().RemoveAll(this);
	CRotating->OnRotatingEnd().RemoveAll(this);

	PostEndPlayEvent.Broadcast();
	PostEndPlayEvent.Clear();
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
	PlayerInputComponent->BindAxis(EVENT_LOOKH, this,
		&ACombatCharacter::OnHorizontalLook);
	PlayerInputComponent->BindAxis(EVENT_LOOKV, this,
		&ACombatCharacter::OnVerticalLook);

	PlayerInputComponent->BindAction(EVENT_JUMP, IE_Pressed, this,
		&ACombatCharacter::OnJumpKeyPressed);
	PlayerInputComponent->BindAction(EVENT_JUMP, IE_Released, this,
		&ACombatCharacter::OnJumpKeyReleased);
	PlayerInputComponent->BindAction(EVENT_ROLL, IE_Pressed, this,
		&ACombatCharacter::OnRollKeyPressed);

	PlayerInputComponent->BindAction(EVENT_TOGGLECOMBAT, IE_Pressed, this,
		&ACombatCharacter::OnToggleCombatKeyPressed);
	PlayerInputComponent->BindAction(EVENT_SPRINT, IE_Pressed, this, 
		&ACombatCharacter::OnSprintKeyPressed);
	PlayerInputComponent->BindAction(EVENT_SPRINT, IE_Released, this, 
		&ACombatCharacter::OnSprintKeyReleased);
	PlayerInputComponent->BindAction(EVENT_ZOOM, IE_Pressed, this,
		&ACombatCharacter::OnZoomKeyPressed);
	PlayerInputComponent->BindAction(EVENT_ZOOM, IE_Released, this,
		&ACombatCharacter::OnZoomKeyReleased);

	PlayerInputComponent->BindAction(EVENT_ATTACK, IE_Pressed, this,
		&ACombatCharacter::OnLightAttackPressed);
	PlayerInputComponent->BindAction(EVENT_HEAVYATTACK, IE_Pressed, this,
		&ACombatCharacter::OnHeavyAttackPressed);
}

UDataTable* ACombatCharacter::GetMontages(EMontage InType)
{
	EMontage LastAction = InType;
	
	TArray<EMontage> LastCommonActions {
		EMontage::Backstabbed,
		EMontage::RollForward,
		EMontage::RollLeft,
		EMontage::RollRight,
		EMontage::RollBackward
	};

	if (LastCommonActions.Contains(LastAction))
	{
		return CommonMontageData;
	}

	if (CEquipment->IsInCombat())
	{
		return CombatTypeMontagesData[CEquipment->GetCombatType()];
	}

	LastCommonActions = { EMontage::DisarmWeapon, EMontage::DrawWeapon };
	if (LastCommonActions.Contains(LastAction))
	{
		return CombatTypeMontagesData[CEquipment->GetCombatType()];
	}

	return CombatTypeMontagesData[ECombat::None];
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

	TimerMangaer.SetTimer(TH_Check, CheckForInteractable, 0.1f, true);
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

void ACombatCharacter::OnZoomKeyPressed()
{
	StartZooming();
}

void ACombatCharacter::OnZoomKeyReleased()
{
	StopZooming();
}

void ACombatCharacter::OnLightAttackPressed()
{
	if (CEquipment->IsInCombat())
	{
		if (IsCombatEqual(ECombat::Melee))
		{
			if (AttemptBackstab() == false)
			{
				CInputBuffer->UpdateKey(EInputBufferKey::LightAttack);
			}
		}
		else
		{
			if (IsCombatEqual(ECombat::Unarmed))
			{
				CInputBuffer->UpdateKey(EInputBufferKey::LightAttack);
			}
		}
	}
	else
	{
		CInputBuffer->UpdateKey(EInputBufferKey::ToggleCombat);
	}
}

void ACombatCharacter::OnHeavyAttackPressed()
{

}

void ACombatCharacter::OnMoveForward(float InAxisValue)
{
	if (IsAlive())
	{
		FVector Forward = UDCSLib::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0));
		DLOG(Log, TEXT("Forward-[%s] InAxisValue-[%f]"), *Forward.ToString(), InAxisValue);
		AddMovementInput(Forward, InAxisValue);
	}
}

void ACombatCharacter::OnMoveRight(float InAxisValue)
{
	if (IsAlive())
	{
		FVector Right = UDCSLib::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, 0));
		DLOG(Log, TEXT("Right-[%s] InAxisValue-[%f]"), *Right.ToString(), InAxisValue);
		AddMovementInput(Right, InAxisValue);
	}
}

void ACombatCharacter::OnHorizontalLook(float InAxisValue)
{
	AddControllerYawInput(HorizontalLookRate * InAxisValue * UDCSLib::GetDTS(this));
	CDynamicTargeting->FindTargetWithAxisInput(GetInputAxisValue(EVENT_LOOKH));
}

void ACombatCharacter::OnVerticalLook(float InAxisValue)
{
	AddControllerPitchInput(VerticalLookRate * InAxisValue * UDCSLib::GetDTS(this));
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
	if (IsStateEqual(EState::Backstabbing))
	{
		if (WP_BackstabbedActor.IsValid())
		{
			FVector Start = GetActorLocation();
			FVector Target = WP_BackstabbedActor->GetActorLocation();

			FRotator ActorRot = GetActorRotation();
			FRotator LookatRot = UDCSLib::FindLookat(Start, Target);
			return UDCSLib::MakeRot(ActorRot.Roll, ActorRot.Pitch, LookatRot.Yaw);
		}
	}
	else
	{
		if (CDynamicTargeting->IsTargetingEnabled())
		{
			if (HasMovementInput())
			{
				if (IsStateEqual(EState::Rolling))
				{
					if (CMontagesManager->GetLastRequestedAction() == EMontage::RollForward)
					{
						return UDCSLib::RotationFromXVector(GetLastMovementInputVector());
					}
				}
			}
			else
			{
				if (IsStateEqual(EState::Rolling))
				{
					if (CMontagesManager->GetLastRequestedAction() == EMontage::RollForward)
					{
						return GetActorRotation();
					}
				}
			}

			FVector Start = GetActorLocation();
			FVector Target = CDynamicTargeting->GetSelectedActor()->GetActorLocation();

			FRotator ActorRot = GetActorRotation();
			FRotator LookatRot = UDCSLib::FindLookat(Start, Target);
			return UDCSLib::MakeRot(ActorRot.Roll, ActorRot.Pitch, LookatRot.Yaw);
		}
	}

	if (HasMovementInput())
	{
		return GetLastMovementInputVector().ToOrientationRotator();
	}

	return GetActorRotation();
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
		MeleeAttack(EMeleeAttack::Light);
		break;
	case EInputBufferKey::HeavyAttack:
		MeleeAttack(EMeleeAttack::Heavy);
		break;
	case EInputBufferKey::ThrustAttack:
		MeleeAttack(EMeleeAttack::Thrust);
		break;
	case EInputBufferKey::SpecialAttack:
		MeleeAttack(EMeleeAttack::Special);
		break;
	case EInputBufferKey::FallingAttack:
		MeleeAttack(EMeleeAttack::Falling);
		break;
	case EInputBufferKey::Roll:
		Roll();
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

void ACombatCharacter::OnInputBufferClosed()
{
	if (IsStateEqual(EState::Disabled))
	{
		return;
	}

	CStateManager->ResetState(0.0f);
}

void ACombatCharacter::OnCombatChanged(bool bChangedValue)
{
	UpdateRotationSettings();

	if (bChangedValue == false)
	{
		StopBlocking();

		ResetAimingMode();
	}
}

void ACombatCharacter::OnRotatingStart()
{
	// TODO: fill function
}

void ACombatCharacter::OnRotatingEnd()
{
	bool bAttacking = IsStateEqual(EState::Attacking);
	bool bUnarmedMelee = IsCombatEqual(ECombat::Unarmed) || IsCombatEqual(ECombat::Melee);
	bool bTargetingEnabled = CDynamicTargeting->IsTargetingEnabled();
	bool bIsInCombat = CEquipment->IsInCombat();

	if (bAttacking && bUnarmedMelee && bTargetingEnabled && bIsInCombat)
	{
		CRotating->StartRotatingWithTime(0.5f, 720.0f);
	}
}

void ACombatCharacter::SetSprint(bool bActivate)
{
	if (bActivate)
	{
		StoredMovementState = CMovementSpeed->GetMovementState();
		CMovementSpeed->SetMovementState(EMovementState::Sprint);
	}
	else
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		if (TimerManager.IsTimerActive(TH_SprintLoop))
		{
			CMovementSpeed->SetMovementState(StoredMovementState);
		}
	}
}

void ACombatCharacter::ResetAimingMode()
{
	StopLookingForward();

	StopAiming();

	StopZooming();

	HideCrossHair();
}

void ACombatCharacter::StartLookingForward()
{
	CStateManager->SetActivity(EActivity::IsLoockingForward, true);
}

void ACombatCharacter::StopLookingForward()
{
	CStateManager->SetActivity(EActivity::IsLoockingForward, false);
}

void ACombatCharacter::StartAiming()
{
	CStateManager->SetActivity(EActivity::IsAimingPressed, true);
}

void ACombatCharacter::StopAiming()
{
	CStateManager->SetActivity(EActivity::IsAimingPressed, false);
}

void ACombatCharacter::StartZooming()
{
	CStateManager->SetActivity(EActivity::IsZooming, true);
}

void ACombatCharacter::StopZooming()
{
	CStateManager->SetActivity(EActivity::IsZooming, false);
}

void ACombatCharacter::HideCrossHair()
{
	// TODO: fill function
}


void ACombatCharacter::Roll()
{
	if (CanRoll() == false)
	{
		return;
	}

	CStateManager->SetState(EState::Rolling);

	UAnimMontage* MontageRoll = GetMontageRoll();
	if (MontageRoll)
	{
		PlayAnimMontage(MontageRoll);
		CExtendedStamina->ModifyStat(-RollStaminaCost, true);
	}
	else
	{
		CStateManager->ResetState(0.0f);
	}
}

void ACombatCharacter::ToggleCombat()
{
	if (IsStateEqual(EState::Idle) == false)
	{
		return;
	}

	CStateManager->SetState(EState::Interacting);
	EMontage Action = CEquipment->IsInCombat() ? EMontage::DisarmWeapon : EMontage::DrawWeapon;
	if (UAnimMontage* Animation = CMontagesManager->GetMontageForAction(Action, 0))
	{
		PlayAnimMontage(Animation);
	}
	else
	{
		CEquipment->ToggleCombat();
		CStateManager->ResetState(0.0f);
	}
}

void ACombatCharacter::StartBlocking()
{
	CStateManager->SetActivity(EActivity::IsBlockingPressed, true);
}

void ACombatCharacter::StopBlocking()
{
	CStateManager->SetActivity(EActivity::IsBlockingPressed, false);
}

void ACombatCharacter::UpdateRotationSettings()
{
	// TODO: Fill Function
}

void ACombatCharacter::MeleeAttack(EMeleeAttack InType)
{
	if (CanMeleeAttack() == false)
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		// 강제 대입
		InType = EMeleeAttack::Falling;
	}

	MeleeAttackType = InType;

	CStateManager->SetState(EState::Attacking);

	FTimerManager& TimerMangaer = GetWorld()->GetTimerManager();
	TimerMangaer.ClearTimer(TH_ResetMeleeAttackCounter);

	UAnimMontage* MeleeAttackMontage = GetMontageMeleeAttack(MeleeAttackType);
}

bool ACombatCharacter::AttemptBackstab()
{
	bool bCanMeleeAttack = CanMeleeAttack();
	if (bCanMeleeAttack)
	{
		auto Start = GetActorLocation();
		auto End = Start + (GetActorForwardVector() * 150.0f);

		TArray<AActor*> Empty;
		FHitResult OutHit;
		bool Result = UDCSLib::LineTraceByChannel(this, Start, End, ETraceTypeQuery::TraceTypeQuery1, false, Empty, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Green, FLinearColor::Red, 5.0f);

		if (Result == false)
		{
			return false;
		}

		if (GetDotProductTo(OutHit.Actor.Get()) >= -0.25f)
		{
			return false;
		}

		UEffectsComponent* Effects = UDCSLib::GetComponent<UEffectsComponent>(*OutHit.Actor);
		if (Effects == nullptr)
		{
			return false;
		}

		float Damage = CStatsManager->GetDamage() * 3.0f;
		EApplyEffectMethod Effect = EApplyEffectMethod::Replace;
		bool Applied = Effects->ApplyBackstabEffect(1.0f, Effect, this, Damage);
		if (Applied == false)
		{
			return false;
		}

		WP_BackstabbedActor = OutHit.Actor.Get();
		CStateManager->SetState(EState::Backstabbing);
	}

	return false;
}

FORCEINLINE bool ACombatCharacter::CanRoll() const
{
	return IsIdleAndNotFalling() && IsEnoughStamina(1.0f);
}

FORCEINLINE bool ACombatCharacter::HasMovementInput() const
{
	return UDCSLib::NotEqual(FVector::ZeroVector, GetCharacterMovement()->GetLastInputVector());
}

FORCEINLINE bool ACombatCharacter::IsEnoughStamina(float InValue) const
{
	return CExtendedStamina->GetCurrentValue() >= InValue;
}

FORCEINLINE bool ACombatCharacter::CanMeleeAttack() const
{
	bool bIdle = IsStateEqual(EState::Idle);
	bool bInCombat = CEquipment->IsInCombat();
	bool bUnarmedOrMelee = IsCombatEqual(ECombat::Unarmed) || IsCombatEqual(ECombat::Melee);
	bool bEnoughStam = IsEnoughStamina(1.0f);
	return bIdle && bInCombat && bUnarmedOrMelee && bEnoughStam;
}

UAnimMontage* ACombatCharacter::GetMontageRoll() const
{
	UAnimMontage* RetVal = nullptr;
	if (HasMovementInput() == false)
	{
		RetVal = CMontagesManager->GetMontageForAction(EMontage::RollBackward, 0);
		if (RetVal)
		{
			return RetVal;
		}
	}

	RetVal = CMontagesManager->GetMontageForAction(EMontage::RollForward, 0);
	return RetVal;
}

UAnimMontage* ACombatCharacter::GetMontageMeleeAttack(EMeleeAttack InType) const
{
	EMontage Action = UDCSLib::CovertMeleeAttackTypeToAction(InType);

	return nullptr;
}

FORCEINLINE UDCSWidget* ACombatCharacter::ShowWidget(EWidgetID InType) const
{
	return UWidgetSystem::Get()->ShowWidget(InType);
}

