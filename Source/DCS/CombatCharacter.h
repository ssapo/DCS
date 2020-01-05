#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structs.h"
#include "Interfaces/IsInteractable.h"
#include "Interfaces/CanBeAttacked.h"
#include "Interfaces/CanGetEffects.h"
#include "Interfaces/CanDesiredRotating.h"
#include "Interfaces/IsArcher.h"
#include "Interfaces/CanOpenUI.h"
#include "CombatCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEquipmentComponent;
class UInventoryComponent;
class URotatingComponent;
class UDissolveComponent;
class UTimelineComponent;
class UStateManagerComponent;
class UMontageManagerComponent;
class UMovementSpeedComponent;
class UInputBufferComponent;
class UExtendedStatComponent;
class UCollisionHandlerComponent;
class UEffectsComponent;
class UStatsManagerComponent;
class UDynamicTargetingComponent;
class UInputComponent;
class UKeybindingsWidget;
class UInGameWidget;
class UDCSWidget;
class UAudioComponent;
enum class EWidgetID;

UCLASS()
class DCS_API ACombatCharacter : public ACharacter
	, public IIsInteractable
	, public ICanBeAttacked
	, public ICanGetEffects
	, public ICanDesiredRotating
	, public IIsArcher
	, public ICanOpenUI
{
	GENERATED_BODY()

public:
	ACombatCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// start interfaces
	void Interact(AActor* Actor) override;
	void OpenedUI() override;
	void ClosedUI() override;

	EAttackResult TakeDamage(const FHitData& HitData) override;
	FName GetInteractionMessage() const override;
	FName GetHeadSocket() const override;
	FName GetBowStringSocketName() const override;
	
	FRotator GetDesiredRotation() const override;
	float GetAimAlpha() const override;

	bool IsAlive() const override;
	bool CanEffectBeApplied(EEffect InType, AActor* Actor) const override;
	bool DoesHoldBowString() override;
	// end interfaces

private:
	void CtorComponents();
	void CtorInitialize(); 
	void CreateHUD();
	void CreateKeyBindings();
	void CreateInGameWidget(); 
	void InitializeComponents();

	void UpdateAimAlpha();
	void SetTimerChecker();

	void OnRollKeyPressed();
	void OnJumpKeyPressed();
	void OnToggleKeyPressed();
	void OnSprintKeyPressed();
	void OnSprintKeyReleased();

	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);

	void OnShowKeyBindings();
	void OnHideKeyBindings();
	void SetSprint(bool bActivate);

	FORCEINLINE UDCSWidget* ShowWidget(EWidgetID InType) const;

	// start Declare Events.
public:
	DECLARE_EVENT(UEquipmentComponent, FOnPostBeginPlay);
	FOnPostBeginPlay& OnPostBeginPlay() { return PostBeginPlayEvent; }

	DECLARE_EVENT(UEquipmentComponent, FOnPostEndPlay);
	FOnPostEndPlay& OnPostEndPlay() { return PostEndPlayEvent; }

private:
	FOnPostBeginPlay PostBeginPlayEvent;
	FOnPostEndPlay PostEndPlayEvent;
	// end Declare Events.

private:
	TWeakObjectPtr<USpringArmComponent> WP_CameraBoom;
	TWeakObjectPtr<UCameraComponent> WP_FollowCamera;
	TWeakObjectPtr<UAudioComponent> WP_EffectsAudio;
	TWeakObjectPtr<UEffectsComponent> WP_Effects;
	TWeakObjectPtr<URotatingComponent> WP_Rotating;
	TWeakObjectPtr<UEquipmentComponent> WP_Equipment;
	TWeakObjectPtr<UInventoryComponent> WP_Inventory;
	TWeakObjectPtr<UDissolveComponent> WP_Dissolve;
	TWeakObjectPtr<UMovementSpeedComponent> WP_MovementSpeed;
	TWeakObjectPtr<UStatsManagerComponent> WP_StatsManager;
	TWeakObjectPtr<UMontageManagerComponent> WP_MontagesManager;
	TWeakObjectPtr<UStateManagerComponent> WP_StateManager;
	TWeakObjectPtr<UInputBufferComponent> WP_InputBuffer;
	TWeakObjectPtr<USceneComponent> WP_ArrowSpawnLocation;
	TWeakObjectPtr<UExtendedStatComponent> WP_ExtendedStamina;
	TWeakObjectPtr<UExtendedStatComponent> WP_ExtendedHealth;
	TWeakObjectPtr<UCollisionHandlerComponent> WP_MeleeCollisionHandler;
	TWeakObjectPtr<UArrowComponent> WP_TargetingArrow;
	TWeakObjectPtr<UDynamicTargetingComponent> WP_DynamicTargeting;

	TWeakObjectPtr<UInGameWidget> WP_InGameWidget;
	TWeakObjectPtr<UKeybindingsWidget> WP_KeyBindingsWidget;
	TWeakObjectPtr<AActor> WP_BackstabbedActor;
	TWeakObjectPtr<AActor> WP_InteractionActor;

	TArray<FName> LeftHandCollisionSockets;
	TArray<FName> RightHandCollisionSockets;
	TArray<FName> RightFootCollisionSockets;
	TArray<FName> LeftFootCollisionSockets;

	EMeleeAttack MeleeAttackType;
	EDirection ReceivedHitDirection;
	EMovementState StoredMovementState;

	float BlockAlpha;
	float HorizontalLockRate;
	float VerticalLookRate;
	float RollStaminaCost;
	float SprintStaminaCost;
	float AimAlpha;
	float SlowMotionTimeDuration;
	float SlowMotionStaminaCost;
	float TargetLagSpeed;
	float ZoomAlpha;
	float ZoomCameraArmLength;
	float InitialCameraArmLength;
	float InitialCameraLagSpeed;

	int32 MeleeAttackCounter;

	bool bAutoZoom;
	bool bIsCrossHairVisible;
	bool bIsInSlowMotion;

	FTimerHandle CheckTimer;
};

