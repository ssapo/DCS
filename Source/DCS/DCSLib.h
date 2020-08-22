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
#include "Kismet/KismetArrayLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DCSLib.generated.h"

class APawn;
class AActor;
class UActorComponent;
class UCanvasPanelSlot;
class UWidget;
class UImage;
class UUserWidget;
class APlayerController;
class ADCSGameModeBase;
class AAIController;
class ADCSAIController;
namespace EDrawDebugTrace { enum Type; }

UCLASS()
class DCS_API UDCSLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	template <typename T = UActorComponent>
	static T* GetComponent(APawn* Owner);
	
	template <typename T = UActorComponent>
	static T * GetComponent(const APawn& Owner);

	template <typename T = UActorComponent>
	static T * GetComponent(AActor* Owner);

	template <typename T = UActorComponent>
	static T * GetComponent(const AActor& Owner);

	template <typename T = UActorComponent>
	static T* GetComponent(UUserWidget* WCO);

	template <typename T = UUserWidget>
	static TArray<T*> GetWidgets(UObject* WCO);

	template <typename T = AGameMode>
	static T * GetGameMode(const UObject * WCO);

	template <typename T>
	static void SetArrayElem(const T& item, TArray<T>& item_array, int32 index);

	static FString GetStringAsEnum(const FString& TypeName, int32 EnumValue);

	static bool IsValidClass(UClass* InClass);

	static UCanvasPanelSlot* SlotAsCanvasSlot(UWidget* InWidget);

	static FVector2D GetViewportSize(UObject* WCO);

	static bool CapsuleTraceForObjects(UObject* WCO, const FVector Start, const FVector End,
		float Radius, float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, 
		bool bTraceComplex, const TArray<AActor*>& Ignores, EDrawDebugTrace::Type DrawDebugType, 
		OUT FHitResult& OutHit, bool bIgnoreSelf, float DrawTime = 5.0f,
		FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green);

	static bool LineTraceByChannel(UObject* WCO, const FVector Start, const FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);

	static float GetDTS(const UObject* WCO);

	static FVector GetForwardVector(FRotator InRot);

	static FVector GetRightVector(FRotator InRot);

	static APlayerController* GetPlayerController(UObject* WCO, int32 Index = 0);
	
	static bool IsInterface(UObject* InObject, TSubclassOf<UInterface> Interface);

	static FRotator InterpTo(FRotator Current, FRotator Target, float DT, float InterpSpeed);

	static float InterpTo(float Current, float Target, float DT, float InterpSpeed);

	static FRotator Delta(FRotator A, FRotator B);

	static FRotator MakeRot(float X, float Y, float Z);

	static FRotator FindLookat(const FVector& Start, const FVector& Target);

	static FRotator RotationFromXVector(const FVector& XVector);

	static bool NotEqual(const FVector& A, const FVector& B);

	static EMontage CovertMeleeAttackTypeToAction(EMeleeAttack InType);

	static float ScaleMeleeAttackStaminaCost(EMeleeAttack InType, float InCost);

	static bool IsItemValid(const FStoredItem* InItem);

	static bool OwnerIsPlayer(AActor* Actor);

	static bool EqualClass(UClass* A, UClass* B);

	static ADCSAIController* GetAIController(AActor* ControllerdActor);

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

template <typename T>
FORCEINLINE T* UDCSLib::GetGameMode(const UObject* WCO)
{
	return Cast<T>(UGameplayStatics::GetGameMode(WCO));
}

template <typename T>
FORCEINLINE void UDCSLib::SetArrayElem(const T& Item, TArray<T>& TargetAray, int32 Index)
{
	if (TargetAray.Num() <= Index)
	{
		int32 Size = Index + 1;
		TargetAray.SetNumZeroed(Size);
	}

	TargetAray[Index] = Item;
}