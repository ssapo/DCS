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
#include "Interfaces/IsMontageManager.h"
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
	, public IIsMontageManager
{
	GENERATED_BODY()

public:
	ACombatCharacter();

	float GetJogSpeed() const;
	bool IsInSlowMotion() const;
	float GetBlockAlpha() const;
	float GetAimAlpha() const override;
	bool IsActivityPure(EActivity InType) const;
	bool IsIdleAndNotFalling() const;
	bool IsStateEqual(EState InType) const;
	bool IsCombatEqual(ECombat InType) const;

	const TTuple<float, float> CalculateLeanAmount() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual UDataTable* GetMontages(EMontage InType) override;

	// start interfaces
	void Interact(AActor* Actor) override;
	void OpenedUI() override;
	void ClosedUI() override;

	EAttackResult TakeDamage(const FHitData& HitData) override;
	FName GetInteractionMessage() const override;
	FName GetHeadSocket() const override;
	FName GetBowStringSocketName() const override;
	
	FRotator GetDesiredRotation() const override;

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
	void OnJumpKeyReleased();
	void OnToggleCombatKeyPressed();
	void OnToggleKeyPressed();
	void OnSprintKeyPressed();
	void OnSprintKeyReleased();
	void OnZoomKeyPressed();
	void OnZoomKeyReleased();
	void OnLightAttackPressed();
	void OnHeavyAttackPressed();

	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);

	void OnShowKeyBindings();
	void OnHideKeyBindings();

	void OnInputBufferConsumed(EInputBufferKey InKey);
	void OnInputBufferClosed();

	void OnCombatChanged(bool bChangedValue);
	void OnRotatingStart();
	void OnRotatingEnd();

	void SetSprint(bool bActivate);
	void ResetAimingMode();

	void StartLookingForward();
	void StopLookingForward();
	void StartAiming();
	void StopAiming();
	void StartZooming();
	void StopZooming();
	void StartBlocking();
	void StopBlocking();

	void HideCrossHair();
	void UpdateRotationSettings();
	void ResetMeleeAttackCounter();

	//start OnInputBufferConsumed
	void MeleeAttack(EMeleeAttack InType);
	void Roll();
	void ToggleCombat();

	//end OnInputBufferConsumed
	bool AttemptBackstab();

	UAnimMontage* GetMontageRoll() const;
	UAnimMontage* GetMontageMeleeAttack(EMeleeAttack InType) const;

	bool CanRoll() const;
	bool HasMovementInput() const;
	bool IsEnoughStamina(float InValue) const;
	bool CanMeleeAttack() const;

	UDCSWidget* ShowWidget(EWidgetID InType) const;

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
	UPROPERTY(EditAnywhere)
		TMap<ECombat, UDataTable*> CombatTypeMontagesData;
	
	UPROPERTY(EditAnywhere)
		UDataTable* CommonMontageData;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		USpringArmComponent* CCameraBoom;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UCameraComponent* CFollowCamera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UAudioComponent* CEffectsAudio;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UEffectsComponent* CEffects;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		URotatingComponent* CRotating;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UEquipmentComponent* CEquipment;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UInventoryComponent* CInventory;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UDissolveComponent* CDissolve;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UMovementSpeedComponent* CMovementSpeed;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UStatsManagerComponent* CStatsManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UMontageManagerComponent* CMontagesManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UStateManagerComponent* CStateManager;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UInputBufferComponent* CInputBuffer;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		USceneComponent* CArrowSpawnLocation;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UExtendedStatComponent* CExtendedStamina;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UExtendedStatComponent* CExtendedHealth;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UCollisionHandlerComponent* CMeleeCollisionHandler;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UArrowComponent* CTargetingArrow;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
		UDynamicTargetingComponent* CDynamicTargeting;

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
	EMontage LastRollDirection;

	float BlockAlpha;
	float HorizontalLookRate;
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

	mutable int32 MeleeAttackCounter;

	bool bAutoZoom;
	bool bIsCrossHairVisible;
	bool bIsInSlowMotion;

	FTimerHandle TH_Check;
	FTimerHandle TH_SprintLoop;
	FTimerHandle TH_ResetMeleeAttackCounter;
};

