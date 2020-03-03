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

#include "Items/ObjectItems/ItemBase.h"

#include "UserWidget.h"
#include "Defines.h"
#include "WidgetSystem.h"
#include "DataTables.h"
#include "TimerManager.h"
#include "DCSLib.h"
#include "DCS.h"
#include "Engine/World.h"

// start public:
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

float ACombatCharacter::GetAimAlpha() const
{
	return AimAlpha;
}

bool ACombatCharacter::IsActivityPure(EActivity InType) const
{
	return CStateManager->GetActivityValue(InType);
}

bool ACombatCharacter::IsIdleAndNotFalling() const
{
	return !GetCharacterMovement()->IsFalling() && IsStateEqual(EState::Idle);
}

bool ACombatCharacter::IsStateEqual(EState InType) const
{
	return CStateManager->GetState() == InType;
}

bool ACombatCharacter::IsCombatEqual(ECombat InType) const
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
	CStateManager->OnActivityChanged().AddUObject(this, &ACombatCharacter::OnActivityChanged);
	CStateManager->OnStateChanged().AddUObject(this, &ACombatCharacter::OnStateChanged);

	if (CF_BlockAlpha)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindDynamic(this, &ACombatCharacter::OnTickBlockAlpha);
		TL_Block.AddInterpFloat(CF_BlockAlpha, TimelineCallback);

		FOnTimelineEvent TimelineFinishedCallback;
		TimelineFinishedCallback.BindDynamic(this, &ACombatCharacter::OnFinishedBlockAlpha);
		TL_Block.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

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

	TL_Block.TickTimeline(DeltaTime);
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
	PlayerInputComponent->BindAction(EVENT_THRUSTATTACK, IE_Pressed, this,
		&ACombatCharacter::OnThrustAttackPressed);
	PlayerInputComponent->BindAction(EVENT_SPECIALATTACK, IE_Pressed, this,
		&ACombatCharacter::OnSpecialAttackPressed);
	PlayerInputComponent->BindAction(EVENT_PARRY, IE_Pressed, this,
		&ACombatCharacter::OnParryAttackPressed);

	PlayerInputComponent->BindAction(EVENT_BLOCK, IE_Pressed, this,
		&ACombatCharacter::OnStartBlocking);
	PlayerInputComponent->BindAction(EVENT_BLOCK, IE_Released, this,
		&ACombatCharacter::OnStopBlocking);
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
		else if(IsCombatEqual(ECombat::Unarmed))
		{
			CInputBuffer->UpdateKey(EInputBufferKey::LightAttack);
		}
	}
	else
	{
		CInputBuffer->UpdateKey(EInputBufferKey::ToggleCombat);
	}
}

void ACombatCharacter::OnHeavyAttackPressed()
{
	if (IsCombatEqual(ECombat::Melee) || IsCombatEqual(ECombat::Unarmed))
	{
		CInputBuffer->UpdateKey(EInputBufferKey::HeavyAttack);
	}
}

void ACombatCharacter::OnThrustAttackPressed()
{
	if (IsCombatEqual(ECombat::Melee))
	{
		CInputBuffer->UpdateKey(EInputBufferKey::ThrustAttack);
	}
}

void ACombatCharacter::OnSpecialAttackPressed()
{
	if (IsCombatEqual(ECombat::Melee))
	{
		CInputBuffer->UpdateKey(EInputBufferKey::SpecialAttack);
	}
}

void ACombatCharacter::OnParryAttackPressed()
{
	if (IsCombatEqual(ECombat::Melee) || IsCombatEqual(ECombat::Unarmed))
	{
		CInputBuffer->UpdateKey(EInputBufferKey::Parry);
	}
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

void ACombatCharacter::OnActivityChanged(EActivity InActivity, bool InValue)
{
	switch (InActivity)
	{
	case EActivity::IsBlockingPressed:
		UpdateBlocking();
		break;
	case EActivity::IsAimingPressed:
		break;
	case EActivity::IsImmortal:
		break;
	case EActivity::CantBeInterrupted:
		break;
	case EActivity::IsLoockingForward:
		break;
	case EActivity::CanParryHit:
		break;
	case EActivity::IsZooming:
		break;
	default:
		break;
	}
}

void ACombatCharacter::OnStateChanged(EState PrevState, EState NewState)
{
	UpdateBlocking();
}

void ACombatCharacter::OnCombatChanged(bool bChangedValue)
{
	UpdateRotationSettings();

	if (bChangedValue == false)
	{
		OnStopBlocking();

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

void ACombatCharacter::OnStartBlocking()
{
	CStateManager->SetActivity(EActivity::IsBlockingPressed, true);
}

void ACombatCharacter::OnStopBlocking()
{
	CStateManager->SetActivity(EActivity::IsBlockingPressed, false);
}

void ACombatCharacter::UpdateBlocking()
{
	if (CanBlock())
	{
		TL_Block.Play();
	}
	else
	{
		TL_Block.Reverse();
	}
	DFP(FColor::Green);
}

void ACombatCharacter::OnTickBlockAlpha(float InAlpha)
{
	BlockAlpha = InAlpha;
	DPRINT(TEXT("Value %f"), BlockAlpha);
}

void ACombatCharacter::OnFinishedBlockAlpha()
{
	if (TL_Block.IsReversing())
	{
		CExtendedStamina->ChangeRegenPercent(25.0f);
	}
	else
	{
		CExtendedStamina->ChangeRegenPercent(100.0f);
	}
}

void ACombatCharacter::UpdateRotationSettings()
{
	// TODO: Fill Function
}

void ACombatCharacter::ResetMeleeAttackCounter()
{
	MeleeAttackCounter = 0;
}

void ACombatCharacter::MeleeAttack(EMeleeAttack InType)
{
	if (CanMeleeAttack() == false)
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		InType = EMeleeAttack::Falling;
	}

	MeleeAttackType = InType;
	CStateManager->SetState(EState::Attacking);

	FTimerManager& TimerMangaer = GetWorld()->GetTimerManager();
	TimerMangaer.ClearTimer(TH_ResetMeleeAttackCounter);

	UAnimMontage* MeleeAttackMontage = GetMontageMeleeAttack(MeleeAttackType);
	if (MeleeAttackMontage)
	{
		float AttackSpeed = CStatsManager->GetStatValue(EStat::AttackSpeed, true);
		float Duration = PlayAnimMontage(MeleeAttackMontage, AttackSpeed);
		TimerMangaer.SetTimer(TH_ResetMeleeAttackCounter, Duration, false);

		float AttackStam = CStatsManager->GetStatValue(EStat::MeleeAttackStaminaCost, true);
		AttackStam = UDCSLib::ScaleMeleeAttackStaminaCost(MeleeAttackType, AttackStam);

		CExtendedStamina->ModifyStat(AttackStam, true);
	}
	else
	{
		CStateManager->ResetState(0.0f);
		ResetMeleeAttackCounter();
	}
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

bool ACombatCharacter::CanRoll() const
{
	return IsIdleAndNotFalling() && IsEnoughStamina(1.0f);
}

bool ACombatCharacter::HasMovementInput() const
{
	return UDCSLib::NotEqual(FVector::ZeroVector, GetCharacterMovement()->GetLastInputVector());
}

bool ACombatCharacter::IsEnoughStamina(float InValue) const
{
	return CExtendedStamina->GetCurrentValue() >= InValue;
}

bool ACombatCharacter::CanMeleeAttack() const
{
	bool bIdle = IsStateEqual(EState::Idle);
	bool bInCombat = CEquipment->IsInCombat();
	bool bUnarmedOrMelee = IsCombatEqual(ECombat::Unarmed) || IsCombatEqual(ECombat::Melee);
	bool bEnoughStam = IsEnoughStamina(1.0f);
	return bIdle && bInCombat && bUnarmedOrMelee && bEnoughStam;
}

bool ACombatCharacter::CanBlock() const
{
	bool bActivity = IsActivityPure(EActivity::IsBlockingPressed);
	bool bIdleAndNotFalling = IsIdleAndNotFalling();
	bool bCanBlock = CEquipment->CanBlock();
	bool bEnoughStam = IsEnoughStamina(5.0f);
	return bActivity && bIdleAndNotFalling && bCanBlock && bEnoughStam;
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
	int32 LastIndex = CMontagesManager->GetMontageActionLastIndex(Action);
	int32 ActionIndex = LastIndex;
	if (MeleeAttackCounter < LastIndex)
	{
		ActionIndex = MeleeAttackCounter;
	}

	UAnimMontage* Montage = CMontagesManager->GetMontageForAction(Action, ActionIndex);
	if (Montage == nullptr)
	{
		return nullptr;
	}

	++MeleeAttackCounter;
	if (MeleeAttackCounter > LastIndex)
	{
		MeleeAttackCounter = 0;
	}

	return Montage;
}

FORCEINLINE UDCSWidget* ACombatCharacter::ShowWidget(EWidgetID InType) const
{
	return UWidgetSystem::Get()->ShowWidget(InType);
}

