// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WidgetBlueprintLibrary.h"
#include "WidgetLayoutLibrary.h"
#include "CanvasPanelSlot.h"
#include "UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Structs.h"
#include "DCSLib.generated.h"

class APawn;
class UActorComponent;
class UCanvasPanelSlot;
class UWidget;
class UImage;
class UUserWidget;
class APlayerController;
namespace EDrawDebugTrace { enum Type; }

UCLASS()
class DCS_API UDCSLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	template <typename T = UActorComponent>
	static FORCEINLINE T* GetComponent(APawn* Owner);
	
	template <typename T = UActorComponent>
	static FORCEINLINE T * GetComponent(const APawn& Owner);

	template <typename T = UActorComponent>
	static FORCEINLINE T * GetComponent(AActor* Owner);

	template <typename T = UActorComponent>
	static FORCEINLINE T * GetComponent(const AActor& Owner);

	template <typename T = UActorComponent>
	static FORCEINLINE T* GetComponent(UUserWidget* WCO);

	template <typename T = UUserWidget>
	static FORCEINLINE TArray<T*> GetWidgets(UObject* WCO);

	static FORCEINLINE FString GetStringAsEnum(const FString& TypeName, int32 EnumValue);

	static FORCEINLINE bool IsValidClass(UClass* InClass);

	static FORCEINLINE UCanvasPanelSlot* SlotAsCanvasSlot(UWidget* InWidget);

	static FORCEINLINE FVector2D GetViewportSize(UObject* WCO);

	static FORCEINLINE bool CapsuleTraceForObjects(UObject* WCO, const FVector Start, const FVector End,
		float Radius, float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, 
		bool bTraceComplex, const TArray<AActor*>& Ignores, EDrawDebugTrace::Type DrawDebugType, 
		OUT FHitResult& OutHit, bool bIgnoreSelf, float DrawTime = 5.0f,
		FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green);

	static FORCEINLINE bool LineTraceByChannel(UObject* WCO, const FVector Start, const FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);

	static FORCEINLINE float GetDTS(const UObject* WCO);

	static FORCEINLINE FVector GetForwardVector(FRotator InRot);

	static FORCEINLINE FVector GetRightVector(FRotator InRot);

	static FORCEINLINE APlayerController* GetPlayerController(UObject* WCO, int32 Index = 0);
	
	static FORCEINLINE bool IsInterface(UObject* InObject, TSubclassOf<UInterface> Interface);

	static FORCEINLINE FRotator InterpTo(FRotator Current, FRotator Target, float DT, float InterpSpeed);

	static FORCEINLINE float InterpTo(float Current, float Target, float DT, float InterpSpeed);

	static FORCEINLINE FRotator Delta(FRotator A, FRotator B);

	static FORCEINLINE FRotator MakeRot(float X, float Y, float Z);

	static FORCEINLINE FRotator FindLookat(const FVector& Start, const FVector& Target);

	static FORCEINLINE FRotator RotationFromXVector(const FVector& XVector);

	static FORCEINLINE bool NotEqual(const FVector& A, const FVector& B);

	static FORCEINLINE EMontage CovertMeleeAttackTypeToAction(EMeleeAttack InType);

public:
	static FString INV_STRING;
	static int32 INV_INDEX;
};

template <typename T /*= UActorComponent*/>
FORCEINLINE T* UDCSLib::GetComponent(APawn* Owner)
{
	if (Owner)
	{
		return Cast<T>(Owner->GetComponentByClass(T::StaticClass()));
	}

	return nullptr;
}

template <typename T /*= UActorComponent*/>
FORCEINLINE T* UDCSLib::GetComponent(AActor* Owner)
{
	if (Owner)
	{
		return Cast<T>(Owner->GetComponentByClass(T::StaticClass()));
	}

	return nullptr;
}

template <typename T /*= UActorComponent*/>
FORCEINLINE T* UDCSLib::GetComponent(UUserWidget* InUserWidget)
{
	if (InUserWidget)
	{
		return GetComponent<T>(InUserWidget->GetOwningPlayerPawn());
	}
	
	return nullptr;
}

template <typename T /*= UActorComponent*/>
FORCEINLINE T* UDCSLib::GetComponent(const APawn& Owner)
{
	return Cast<T>(Owner.GetComponentByClass(T::StaticClass()));
}

template <typename T /*= UActorComponent*/>
FORCEINLINE T* UDCSLib::GetComponent(const AActor& Owner)
{
	return Cast<T>(Owner.GetComponentByClass(T::StaticClass()));
}

template <typename T>
FORCEINLINE TArray<T*> UDCSLib::GetWidgets(UObject* WCO)
{
	TArray<T*> ReturnWidgets;

	if (WCO == nullptr)
	{
		ensureMsgf(WCO, TEXT("Context should not be null."));
		return ReturnWidgets;
	}

	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WCO, FoundWidgets, T::StaticClass());
	
	if (FoundWidgets.Num() > 0)
	{
		for (const auto& E : FoundWidgets)
		{
			if (T* Inst = Cast<T>(E))
			{
				ReturnWidgets.Add(Inst);
			}
		}
	}

	return ReturnWidgets;
}

FORCEINLINE FString UDCSLib::GetStringAsEnum(const FString& TypeName, int32 EnumValue)
{
	const UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *TypeName, true);
	if (Enum == false)
	{
		return INV_STRING;
	}
	return Enum->GetNameStringByIndex(EnumValue);
}

FORCEINLINE bool UDCSLib::IsValidClass(UClass* InClass)
{
	return UKismetSystemLibrary::IsValidClass(InClass);
}

FORCEINLINE UCanvasPanelSlot* UDCSLib::SlotAsCanvasSlot(UWidget* InWidget)
{
	return UWidgetLayoutLibrary::SlotAsCanvasSlot(InWidget);
}

FORCEINLINE FVector2D UDCSLib::GetViewportSize(UObject* WCO)
{
	return UWidgetLayoutLibrary::GetViewportSize(WCO);
}

FORCEINLINE bool UDCSLib::CapsuleTraceForObjects(UObject* WCO, const FVector Start, const FVector End, float Radius, float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, bool bTraceComplex, const TArray<AActor*>& Ignores, EDrawDebugTrace::Type DrawDebugType, OUT FHitResult& OutHit, bool bIgnoreSelf, float DrawTime /*= 5.0f*/, FLinearColor TraceColor /*= FLinearColor::Red*/, FLinearColor TraceHitColor /*= FLinearColor::Green*/)
{
	return  UKismetSystemLibrary::CapsuleTraceSingleForObjects(
		WCO, Start, End, Radius, HalfHeight, ObjectTypes, bTraceComplex, Ignores,
		DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
}

FORCEINLINE bool UDCSLib::LineTraceByChannel(UObject* WCO, const FVector Start, const FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	return UKismetSystemLibrary::LineTraceSingle(
		WCO, Start, End, TraceChannel, bTraceComplex, ActorsToIgnore, 
		DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
}

FORCEINLINE float UDCSLib::GetDTS(const UObject* WCO)
{
	return UGameplayStatics::GetWorldDeltaSeconds(WCO);
}

FORCEINLINE FVector UDCSLib::GetForwardVector(FRotator InRot)
{
	return UKismetMathLibrary::GetForwardVector(InRot);
}

FORCEINLINE FVector UDCSLib::GetRightVector(FRotator InRot)
{
	return UKismetMathLibrary::GetRightVector(InRot);
}

FORCEINLINE APlayerController* UDCSLib::GetPlayerController(UObject* WCO, int32 Index)
{
	return UGameplayStatics::GetPlayerController(WCO, Index);
}

FORCEINLINE bool UDCSLib::IsInterface(UObject* InObject, TSubclassOf<UInterface> Interface)
{
	return UKismetSystemLibrary::DoesImplementInterface(InObject, Interface);
}

FORCEINLINE FRotator UDCSLib::InterpTo(FRotator Current, FRotator Target, float DT, float InterpSpeed)
{
	return UKismetMathLibrary::RInterpTo_Constant(Current, Target, DT, InterpSpeed);
}

FORCEINLINE float UDCSLib::InterpTo(float Current, float Target, float DT, float InterpSpeed)
{
	return UKismetMathLibrary::FInterpTo_Constant(Current, Target, DT, InterpSpeed);
}

FORCEINLINE FRotator UDCSLib::Delta(FRotator A, FRotator B)
{
	return UKismetMathLibrary::NormalizedDeltaRotator(A, B);
}

FORCEINLINE FRotator UDCSLib::MakeRot(float Roll, float Pitch, float Yaw)
{
	return UKismetMathLibrary::MakeRotator(Roll, Pitch, Yaw);
}

FORCEINLINE FRotator UDCSLib::FindLookat(const FVector& Start, const FVector& Target)
{
	return UKismetMathLibrary::FindLookAtRotation(Start, Target);
}

FORCEINLINE FRotator UDCSLib::RotationFromXVector(const FVector& XVector)
{
	return UKismetMathLibrary::Conv_VectorToRotator(XVector);
}

FORCEINLINE bool UDCSLib::NotEqual(const FVector& A, const FVector& B)
{
	return UKismetMathLibrary::NotEqual_VectorVector(A, B, 0.0001f);
}

FORCEINLINE EMontage UDCSLib::CovertMeleeAttackTypeToAction(EMeleeAttack InType)
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

