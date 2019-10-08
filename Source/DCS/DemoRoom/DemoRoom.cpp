// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoRoom.h"
#include "Kismet/KismetMathLibrary.h"

void ADemoRoom::OnConstruction(const FTransform& Transform)
{
	Initialize();
}

void ADemoRoom::Initialize()
{
	// Initialize some values.
	ThisRoomSize = 0;
	OffsetList.Empty();
	RoomSizes.Empty();

	// Loop for each room.
	NumberofRooms = FMath::Clamp<int32>(NumberofRooms, 1, 30);
	for (int32 I = 0; I < NumberofRooms; ++I)
	{
		MainLoopIndex = I;

		// Get the size of this room. Either the base or the override.
		if (PerRoomProperties.IsValidIndex(MainLoopIndex))
		{
			auto RoomProperty = PerRoomProperties[MainLoopIndex];
			
			int RoomSize = RoomProperty.OverrideSize;
			bool IsBiggerThan0 = RoomSize > 0;

			RoomSize = UKismetMathLibrary::SelectInt(RoomSize, BaseRoomSize, IsBiggerThan0);
			ThisRoomSize = RoomSize;
		}

		// Calculate the offset for this room.
		Offset = 0.0f;
		ThisOffset = ThisRoomSize * SectionWidth.X + TrimWidth.X;
		OffsetList.Add(ThisOffset);
		for (auto Each : OffsetList)
		{
			Offset += Each;
		}

		// Add Components of the Room.
		AddRoomNames(Offset, ThisRoomSize, MainLoopIndex);
		AddLoopSections(FVector::OneVector, Offset, ThisRoomSize, MainLoopIndex);
		AddTrim(FVector::OneVector, Offset, ThisRoomSize, MainLoopIndex);
		AddWall(FVector::OneVector, Offset, ThisRoomSize, MainLoopIndex);
		AddLights(Offset, ThisRoomSize, MainLoopIndex);
		AddOpenTrim(FVector::OneVector, Offset, ThisRoomSize, MainLoopIndex);

		if (bMirrorRoom)
		{
			AddLoopSections(FVector(1, -1, 1), Offset, ThisRoomSize, MainLoopIndex);
			AddTrim(FVector(1, -1, 1), Offset, ThisRoomSize, MainLoopIndex);
			AddWall(FVector(1, -1, 1), Offset, ThisRoomSize, MainLoopIndex);
			AddOpenTrim(FVector(1, -1, 1), Offset, ThisRoomSize, MainLoopIndex);
		}
		else
		{
			AddBackWall(Offset, ThisRoomSize);
			AddClamp(Offset, ThisRoomSize, MainLoopIndex);
		}
	}
}

void ADemoRoom::AddLoopSections(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex)
{
	// Start loop depending on room size

	// Add and transform loop mesh
	bool bCondition = true;
	if (bCondition)
	{
		// Set loop mesh
	}
	else
	{
		// Set last loop mesh

	}
	// Set Color
}

void ADemoRoom::AddBackWall(float InOffset, int32 InRoomSize)
{
	// Add Wall mesh

	// Different wall if double height room
}

void ADemoRoom::AddClamp(float InOffset, int32 InRoomSize, int32 InIndex)
{
	// Add and transform Clamp mesh

	// Last mesh and transformation
}

void ADemoRoom::AddTrim(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex)
{
	// Add mesh

	// Add and set transformation for last mesh
}

void ADemoRoom::AddWall(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex)
{
	// Select mesh

	// Then-0 AddWall
	// Add Glass walls

	// Then-1 Add 2nd Wall
	// Add Glass walls

	// Then-2 Add room divider

	// Then-3 Front Door
}

void ADemoRoom::AddLights(float InOffset, int32 InRoomSize, int32 InIndex)
{
	// Add lights if true

	// Start Loop and Offset Transformation

	// Set Light Parameters
}

void ADemoRoom::AddOpenTrim(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex)
{

}

void ADemoRoom::AddRoomNames(float InOffset, int32 InRoomSize, int32 InIndex)
{

}
