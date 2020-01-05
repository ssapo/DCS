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
