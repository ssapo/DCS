#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Structs.h"
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
enum class EWidgetID;

UCLASS()
class DCS_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	constexpr static int32 ZOrder_InGame = 0;
	constexpr static int32 ZOrder_KeyBindings = 1;

public:
	ACombatCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	void CtorComponents();
	void CtorInitialize();
	void CreateHUD();
	void UpdateAimAlpha();

	void CreateKeyBindings();
	void CreateInGameWidget();

	void ShowKeyBindings();
	void HideKeyBindings();

	FORCEINLINE UDCSWidget* ShowWidget(EWidgetID InType) const;

private:
	TWeakObjectPtr<USpringArmComponent> CameraBoom;
	TWeakObjectPtr<UCameraComponent> FollowCamera;
	TWeakObjectPtr<UAudioComponent> EffectsAudio;
	TWeakObjectPtr<UEffectsComponent> Effects;
	TWeakObjectPtr<URotatingComponent> Rotating;
	TWeakObjectPtr<UEquipmentComponent> Equipment;
	TWeakObjectPtr<UInventoryComponent> Inventory;
	TWeakObjectPtr<UDissolveComponent> Dissolve;
	TWeakObjectPtr<UMovementSpeedComponent> MovementSpeed;
	TWeakObjectPtr<UStatsManagerComponent> StatsManager;
	TWeakObjectPtr<UMontageManagerComponent> MontagesManager;
	TWeakObjectPtr<UStateManagerComponent> StateManager;
	TWeakObjectPtr<UInputBufferComponent> InputBuffer;
	TWeakObjectPtr<USceneComponent> ArrowSpawnLocation;
	TWeakObjectPtr<UExtendedStatComponent> ExtendedStamina;
	TWeakObjectPtr<UExtendedStatComponent> ExtendedHealth;
	TWeakObjectPtr<UCollisionHandlerComponent> MeleeCollisionHandler;
	TWeakObjectPtr<UArrowComponent> TargetingArrow;
	TWeakObjectPtr<UDynamicTargetingComponent> DynamicTargeting;

	TWeakObjectPtr<UInGameWidget> WP_InGameWidget;
	TWeakObjectPtr<UKeybindingsWidget> WP_KeyBindingsWidget;
	TWeakObjectPtr<AActor> BackstabbedActor;
	TWeakObjectPtr<AActor> InteractionActor;

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
};
