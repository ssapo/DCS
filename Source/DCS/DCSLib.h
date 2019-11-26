// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DCSLib.generated.h"

class APawn;
class UActorComponent;
class UCanvasPanelSlot;
class UWidget;

UCLASS()
class DCS_API UDCSLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool IsValidClass(UClass* InClass);

	template <typename T = UActorComponent>
	static T* GetComponent(APawn* InClass);

	static UCanvasPanelSlot* SlotAsCanvasSlot(UWidget* InWidget);

	static FVector2D GetViewportSize(UObject* WowldContextObject);
};

template <typename T>
T* UDCSLib::GetComponent(APawn* InPawn)
{
	if (InPawn)
	{
		return Cast<T>(InPawn->GetComponentByClass(T::StaticClass()));
	}

	return nullptr;
}
