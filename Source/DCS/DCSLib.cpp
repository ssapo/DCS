#include "DCSLib.h"
#include "Defines.h"

FString UDCSLib::INV_STRING = INVALID_STRING;
int32 UDCSLib::INV_INDEX = INVALID_INDEX;

FString UDCSLib::GetStringAsEnum(const FString& TypeName, int32 EnumValue)
{
	const UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *TypeName, true);
	if (Enum == false)
	{
		return INV_STRING;
	}
	return Enum->GetNameStringByIndex(EnumValue);
}

bool UDCSLib::IsValidClass(UClass* InClass)
{
	return UKismetSystemLibrary::IsValidClass(InClass);
}

UCanvasPanelSlot* UDCSLib::SlotAsCanvasSlot(UWidget* InWidget)
{
	return UWidgetLayoutLibrary::SlotAsCanvasSlot(InWidget);
}

FVector2D UDCSLib::GetViewportSize(UObject* WCO)
{
	return UWidgetLayoutLibrary::GetViewportSize(WCO);
}

bool UDCSLib::CapsuleTraceForObjects(UObject* WCO, const FVector Start, const FVector End, float Radius, float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& Ignores, EDrawDebugTrace::Type DrawDebugType, OUT FHitResult& OutHit, bool bIgnoreSelf, float DrawTime /*= 5.0f*/, FLinearColor TraceColor /*= FLinearColor::Red*/, FLinearColor TraceHitColor /*= FLinearColor::Green*/)
{
	return  UKismetSystemLibrary::CapsuleTraceSingleForObjects(
		WCO, Start, End, Radius, HalfHeight, ObjectTypes, bTraceComplex, Ignores,
		DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
}

bool UDCSLib::LineTraceByChannel(UObject* WCO, const FVector Start, const FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	return UKismetSystemLibrary::LineTraceSingle(
		WCO, Start, End, TraceChannel, bTraceComplex, ActorsToIgnore,
		DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
}

float UDCSLib::GetDTS(const UObject* WCO)
{
	return UGameplayStatics::GetWorldDeltaSeconds(WCO);
}

FVector UDCSLib::GetForwardVector(FRotator InRot)
{
	return UKismetMathLibrary::GetForwardVector(InRot);
}

FVector UDCSLib::GetRightVector(FRotator InRot)
{
	return UKismetMathLibrary::GetRightVector(InRot);
}

APlayerController* UDCSLib::GetPlayerController(UObject* WCO, int32 Index)
{
	return UGameplayStatics::GetPlayerController(WCO, Index);
}

bool UDCSLib::IsInterface(UObject* InObject, TSubclassOf<UInterface> Interface)
{
	return UKismetSystemLibrary::DoesImplementInterface(InObject, Interface);
}

FRotator UDCSLib::InterpTo(FRotator Current, FRotator Target, float DT, float InterpSpeed)
{
	return UKismetMathLibrary::RInterpTo_Constant(Current, Target, DT, InterpSpeed);
}

float UDCSLib::InterpTo(float Current, float Target, float DT, float InterpSpeed)
{
	return UKismetMathLibrary::FInterpTo_Constant(Current, Target, DT, InterpSpeed);
}

FRotator UDCSLib::Delta(FRotator A, FRotator B)
{
	return UKismetMathLibrary::NormalizedDeltaRotator(A, B);
}

FRotator UDCSLib::MakeRot(float Roll, float Pitch, float Yaw)
{
	return UKismetMathLibrary::MakeRotator(Roll, Pitch, Yaw);
}

FRotator UDCSLib::FindLookat(const FVector& Start, const FVector& Target)
{
	return UKismetMathLibrary::FindLookAtRotation(Start, Target);
}

FRotator UDCSLib::RotationFromXVector(const FVector& XVector)
{
	return UKismetMathLibrary::Conv_VectorToRotator(XVector);
}

bool UDCSLib::NotEqual(const FVector& A, const FVector& B)
{
	return UKismetMathLibrary::NotEqual_VectorVector(A, B, 0.0001f);
}

EMontage UDCSLib::CovertMeleeAttackTypeToAction(EMeleeAttack InType)
{
	switch (InType)
	{
	case EMeleeAttack::Light: return EMontage::LightAttack;
	case EMeleeAttack::Heavy: return EMontage::HeavyAttack;
	case EMeleeAttack::Special: return EMontage::SpecialAttack;
	case EMeleeAttack::Thrust: return EMontage::ThrustAttack;
	case EMeleeAttack::Falling: return EMontage::FallingAttack;
	default: return EMontage::None;
	}
}

float UDCSLib::ScaleMeleeAttackStaminaCost(EMeleeAttack InType, float InCost)
{
	switch (InType)
	{
	case EMeleeAttack::Light: return InCost * 1.0f;
	case EMeleeAttack::Heavy: return InCost * 1.75f;
	case EMeleeAttack::Special: return InCost * 1.75f;
	case EMeleeAttack::Thrust: return InCost * 1.75f;
	case EMeleeAttack::Falling: return InCost * 0.75f;
	default: return InCost * 1.0f;
	}
}

bool UDCSLib::IsItemValid(const FStoredItem* InItem)
{
	if (InItem && IsValidClass(InItem->ItemClass))
	{
		return InItem->Amount > 0;
	}

	return false;
}

bool UDCSLib::OwnerIsPlayer(AActor* Owner)
{
	return Owner == UDCSLib::GetPlayerController(Owner, 0);
}

bool UDCSLib::EqualClass(UClass* A, UClass* B)
{
	return UKismetMathLibrary::EqualEqual_ClassClass(A, B);
}

ADCSAIController* UDCSLib::GetAIController(AActor* ControlledActor)
{
	return Cast<ADCSAIController>(UAIBlueprintHelperLibrary::GetAIController(ControlledActor));
}
