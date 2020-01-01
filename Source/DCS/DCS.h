// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LOG_S(Verbosity) UE_LOG(DCS, Verbosity, TEXT("%s"), *CALLINFO)
#define LOG(Verbosity, Format, ...) UE_LOG(DCS, Verbosity, TEXT("%s%s"), *DCS_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define PRINT(Format, ...) { if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(Format, ##__VA_ARGS__), false, FVector2D::UnitVector); } }

#define PRINT_C(Color, Format, ...) { if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 3.0f, Color, FString::Printf(Format, ##__VA_ARGS__), false, FVector2D::UnitVector); } }

#define PRINT_CS(Color, Sec, Format, ...) { if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, Sec, Color, FString::Printf(Format, ##__VA_ARGS__), false, FVector2D::UnitVector); } }
