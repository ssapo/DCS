// Fill out your copyright notice in the Description page of Project Settings.


#include "DCSLib.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "UserWidget.h"
#include "CanvasPanelSlot.h"
#include "WidgetLayoutLibrary.h"

bool UDCSLib::IsValidClass(UClass* InClass)
{
	return UKismetSystemLibrary::IsValidClass(InClass);
}

UCanvasPanelSlot* UDCSLib::SlotAsCanvasSlot(UWidget* InWidget)
{
	return UWidgetLayoutLibrary::SlotAsCanvasSlot(InWidget);
}

FVector2D UDCSLib::GetViewportSize(UObject* WowldContextObject)
{
	return UWidgetLayoutLibrary::GetViewportSize(WowldContextObject);
}

