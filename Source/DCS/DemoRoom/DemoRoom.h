// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "DemoRoom.generated.h"

UCLASS(Blueprintable)
class DCS_API ADemoRoom : public AActor
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void Initialize();
	void AddLoopSections(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex);
	void AddBackWall(float InOffset, int32 InRoomSize);
	void AddClamp(float InOffset, int32 InRoomSize, int32 InIndex);
	void AddTrim(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex);
	void AddWall(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex);
	void AddLights(float InOffset, int32 InRoomSize, int32 InIndex);
	void AddOpenTrim(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex);
	void AddRoomNames(float InOffset, int32 InRoomSize, int32 InIndex);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMesh* LoopMesh;

	TArray<FDemoRoomInfo> PerRoomProperties;
	TArray<EDemoRoom> RoomTypes;
	TArray<FText> RoomNames;
	TArray<int32> RoomSizes;
	TArray<float> OffsetList;

	FVector SectionWidth;
	FVector TrimWidth;
	FText EmptyText;

	float Offset;
	float ThisOffset;
	float LightRaidus;
	float Brightness;

	int32 BaseRoomSize;
	int32 NumberofRooms;
	int32 TotalSegments;
	int32 ThisRoomSize;
	int32 MainLoopIndex;

	bool bCastShadows;
	bool bMirrorRoom;
	bool bLights;
	bool bDoubleHeight;
	bool bOpenRoof;
	bool bSwitchColor;
	bool bGlassWalls;
	bool bFrontDoor;
	bool bOpenBack;
};
