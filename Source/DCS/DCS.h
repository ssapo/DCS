// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(DCSLog, Log, All);

#define DCALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define DLOG_S(Verbosity) UE_LOG(DCSLog, Verbosity, TEXT("%s"), *DCALLINFO)
#define DLOG(Verbosity, Format, ...) UE_LOG(DCSLog, Verbosity, TEXT("%s%s"), *DCALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define DPRINT_D(Key, Color, Sec, NewerOnTop, Format, ...) { if(GEngine) { GEngine->AddOnScreenDebugMessage(Key, Sec, Color, FString::Printf(Format, ##__VA_ARGS__), NewerOnTop, FVector2D::UnitVector); } }

#define DFP(Color) DPRINT_D(-1, Color, 3.0f, true, TEXT("%s"), *DCALLINFO)

#define DPRINT(Format, ...) DPRINT_D(-1, FColor::Green, 3.0f, true,  Format, ##__VA_ARGS__)

#define DPRINT_C(Color, Format, ...) DPRINT_D(-1, Color, 3.0f, true, Format, ##__VA_ARGS__)

#define DPRINT_CS(Color, Sec, Format, ...) DPRINT_D(-1, Color, Sec, true, Format, ##__VA_ARGS__)

#define T2T(Type) TEXT(#Type)