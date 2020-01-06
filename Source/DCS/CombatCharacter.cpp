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
#include "Define.h"
#include "WidgetSystem.h"
#include "DataTables.h"
#include "Widgets/KeybindingsWidget.h"
#include "Widgets/InGameWidget.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "DCSLib.h"
#include "DCS.h"

ACombatCharacter::ACombatCharacter()
{
	CtorComponents();

	CtorInitialize();

	PrimaryActorTick.bCanEverTick = true;
}

void ACombatCharacter::CtorComponents()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	ensure(MeshComponent != nullptr);

	WP_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	ensure(WP_CameraBoom.IsValid() == true);
	WP_CameraBoom->SetupAttachment(RootComponent);

	WP_TargetingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TargetingArrow"));
	ensure(WP_TargetingArrow.IsValid() == true);
	WP_TargetingArrow->SetupAttachment(RootComponent);

	WP_FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	ensure(WP_FollowCamera.IsValid() == true);
	WP_FollowCamera->SetupAttachment(WP_CameraBoom.Get());

	WP_EffectsAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("EffectsAudio"));
	ensure(WP_EffectsAudio.IsValid() == true);
	WP_EffectsAudio->SetupAttachment(MeshComponent);

	WP_ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowSpawnLocation"));
	ensure(WP_ArrowSpawnLocation.IsValid() == true);
	WP_ArrowSpawnLocation->SetupAttachment(MeshComponent);

	WP_StateManager = CreateDefaultSubobject<UStateManagerComponent>(TEXT("StateManager"));
	ensure(WP_StateManager.IsValid() == true);

	WP_InputBuffer = CreateDefaultSubobject<UInputBufferComponent>(TEXT("InputBuffer"));
	ensure(WP_InputBuffer.IsValid() == true);

	WP_MeleeCollisionHandler = CreateDefaultSubobject<UCollisionHandlerComponent>(TEXT("MeleeCollisionHandler"));
	ensure(WP_MeleeCollisionHandler.IsValid() == true);

	WP_ExtendedHealth = CreateDefaultSubobject<UExtendedStatComponent>(TEXT("ExtendedHealth"));
	ensure(WP_ExtendedHealth.IsValid() == true);

	WP_ExtendedStamina = CreateDefaultSubobject<UExtendedStatComponent>(TEXT("ExtendedStamina"));
	ensure(WP_ExtendedStamina.IsValid() == true);

	WP_MontagesManager = CreateDefaultSubobject<UMontageManagerComponent>(TEXT("MontagesManager"));
	ensure(WP_MontagesManager.IsValid() == true);

	WP_Effects = CreateDefaultSubobject<UEffectsComponent>(TEXT("Effects"));
	ensure(WP_Effects.IsValid() == true);

	WP_MovementSpeed = CreateDefaultSubobject<UMovementSpeedComponent>(TEXT("MovementSpeed"));
	ensure(WP_MovementSpeed.IsValid() == true);

	WP_StatsManager = CreateDefaultSubobject<UStatsManagerComponent>(TEXT("StatsManager"));
	ensure(WP_StatsManager.IsValid() == true);

	WP_Dissolve = CreateDefaultSubobject<UDissolveComponent>(TEXT("Dissolve"));
	ensure(WP_Dissolve.IsValid() == true);

	WP_Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	ensure(WP_Inventory.IsValid() == true);

	WP_Equipment = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
	ensure(WP_Equipment.IsValid() == true);

	WP_Rotating = CreateDefaultSubobject<URotatingComponent>(TEXT("Rotating"));
	ensure(WP_Rotating.IsValid() == true);
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

	WP_CameraBoom->TargetArmLength = InitialCameraArmLength;
	WP_CameraBoom->CameraLagSpeed = InitialCameraLagSpeed;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeComponents();

	CreateHUD();

	SetTimerChecker();

	PostBeginPlayEvent.Broadcast();
	PostBeginPlayEvent.Clear();
}

void ACombatCharacter::InitializeComponents()
{
	WP_DynamicTargeting->Initialize(*WP_TargetingArrow);
	WP_StatsManager->Initialize();
	WP_Equipment->Initialize();
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
	DLOG_S(Log);

	if (WP_InputBuffer.IsValid())
	{
		WP_InputBuffer->UpdateKey(EInputBufferKey::Roll);
	}
}

void ACombatCharacter::OnJumpKeyPressed()
{
	DLOG_S(Log);

	if (WP_InputBuffer.IsValid())
	{
		WP_InputBuffer->UpdateKey(EInputBufferKey::Jump);
	}

}

void ACombatCharacter::OnToggleKeyPressed()
{
	DLOG_S(Log);

	if (WP_MovementSpeed.IsValid())
	{
		WP_MovementSpeed->ToggleState();
	}
}

void ACombatCharacter::OnSprintKeyPressed()
{
	DLOG_S(Log);

	SetSprint(true);
}

void ACombatCharacter::OnSprintKeyReleased()
{
	DLOG_S(Log);

	SetSprint(false);
}

void ACombatCharacter::OnMoveForward(float InAxisValue)
{
	DLOG_S(Log);

	if (IsAlive())
	{
		FRotator Rot = FRotator(0.0f, 0.0f, GetControlRotation().Yaw);
		AddMovementInput(UDCSLib::GetForwardVector(Rot), InAxisValue);
	}
}

void ACombatCharacter::OnMoveRight(float InAxisValue)
{
	DLOG_S(Log);

	if (IsAlive())
	{
		FRotator Rot = FRotator(0.0f, 0.0f, GetControlRotation().Yaw);
		AddMovementInput(UDCSLib::GetRightVector(Rot), InAxisValue);
	}
}

void ACombatCharacter::OnHorizontalLook(float InAxisValue)
{
	DLOG_S(Log);

	AddControllerYawInput(HorizontalLockRate * InAxisValue * UDCSLib::GetDTS(this));
	if (WP_DynamicTargeting.IsValid())
	{
		WP_DynamicTargeting->FindTargetWithAxisInput(InAxisValue);
	}
}

void ACombatCharacter::OnVerticalLook(float InAxisValue)
{
	DLOG_S(Log);

	AddControllerPitchInput(VerticalLookRate * InAxisValue * UDCSLib::GetDTS(this));
}

void ACombatCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

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

	PlayerInputComponent->BindAction(EVENT_KEYBIND, IE_Pressed, this, &ACombatCharacter::OnShowKeyBindings);
	PlayerInputComponent->BindAction(EVENT_KEYBIND, IE_Released, this, &ACombatCharacter::OnHideKeyBindings);

	PlayerInputComponent->BindAxis(EVENT_MOVEFORWARD, this,
		&ACombatCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis(EVENT_MOVERIGHT, this,
		&ACombatCharacter::OnMoveRight);
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

float ACombatCharacter::GetAimAlpha() const
{
	// TODO: fill function
	return 0.0f;
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

FORCEINLINE UDCSWidget* ACombatCharacter::ShowWidget(EWidgetID InType) const
{
	return UWidgetSystem::Get()->ShowWidget(InType);
}

