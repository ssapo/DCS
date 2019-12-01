// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetBlueprintLibrary.h"
#include "UserWidget.h"
#include "DCSLib.generated.h"

class APawn;
class UActorComponent;
class UCanvasPanelSlot;
class UWidget;
class UImage;
class UUserWidget;

UCLASS()
class DCS_API UDCSLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	template <typename T = UActorComponent>
	static T* GetComponent(APawn * InClass);
	
	template <typename T = UUserWidget>
	static T* GetComponent(UUserWidget* WowldContextObject);

	template <typename T =  UUserWidget>
	static TArray<T*> GetWidgets(UObject * WowldContextObject);

	static bool IsValidClass(UClass* InClass);

	static UCanvasPanelSlot* SlotAsCanvasSlot(UWidget* InWidget);

	static FVector2D GetViewportSize(UObject* WowldContextObject);

private:
	static TArray<UUserWidget*> GetWidgets_Impl(UObject* WowldContextObject);
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

template <typename T>
T* UDCSLib::GetComponent(UUserWidget* InUserWidget)
{
	if (InUserWidget)
	{
		return GetComponent<T>(InUserWidget->GetOwningPlayerPawn());
	}
	
	return nullptr;
}

template <typename T>
TArray<T*> UDCSLib::GetWidgets(UObject* WowldContextObject)
{
	TArray<T*> ReturnWidgets;

	if (WowldContextObject == nullptr)
	{
		ensureMsgf(WowldContextObject, TEXT("Context should not be null."));
		return ReturnWidgets;
	}

	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WowldContextObject, FoundWidgets, T::StaticClass());
	
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