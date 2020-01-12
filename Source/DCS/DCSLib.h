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

	static FORCEINLINE bool IsValidClass(UClass* InClass);

	static FORCEINLINE UCanvasPanelSlot* SlotAsCanvasSlot(UWidget* InWidget);

	static FORCEINLINE FVector2D GetViewportSize(UObject* WCO);

	static FORCEINLINE bool CapsuleTraceForObjects(UObject* WCO, const FVector Start, const FVector End,
		float Radius, float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, 
		bool bTraceComplex, const TArray<AActor*>& Ignores, EDrawDebugTrace::Type DrawDebugType, 
		OUT FHitResult& OutHit, bool bIgnoreSelf, float DrawTime = 5.0f,
		FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green);

	static FORCEINLINE float GetDTS(const UObject* WCO);

	static FORCEINLINE FVector GetForwardVector(FRotator InRot);

	static FORCEINLINE FVector GetRightVector(FRotator InRot);

	static FORCEINLINE APlayerController* GetPlayerController(UObject* WCO, int32 Index = 0);
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
