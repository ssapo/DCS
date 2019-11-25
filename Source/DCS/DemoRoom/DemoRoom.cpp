// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoRoom.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"

void ADemoRoom::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

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
	int32 TempSize = FMath::Clamp(InRoomSize, 0, 50);

	// Start loop depending on room size
	for (int32 I = 0; I < TempSize; ++I)
	{
		FVector TempVector = (TrimWidth + SectionWidth * I);
		TempVector.X = (InOffset + TempVector.X);

		FTransform InTransform = UKismetMathLibrary::MakeTransform(TempVector, FRotator::ZeroRotator, InScale);
		UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(this);

		int32 CurrIndex = (I + InRoomSize * InIndex);
		// Add and transform loop mesh
		bool bCondition = RoomTypes.Num() > CurrIndex;
		if (bCondition)
		{
			EDemoRoom CurrType = RoomTypes[CurrIndex];
			UStaticMesh* TempMesh = nullptr;

			switch (CurrType)
			{
			case EDemoRoom::RoofWithHole:
				TempMesh = SM_DemoRoomU_Hole;
				break;
			case EDemoRoom::RoofOpen:
				TempMesh = SM_DemoRoomL;
				break;
			default:
				TempMesh = SM_DemoRoomU;
				break;
			}

			if (bDoubleHeight)
			{
				TempMesh = SM_DemoRoomU2;
			}

			if (bOpenRoof)
			{
				TempMesh = SM_DemoRoomL;
			}

			MeshComp->SetStaticMesh(TempMesh);
		}
		else
		{
			UStaticMesh* TempMesh = SM_DemoRoomU;
			if (bDoubleHeight)
			{
				TempMesh = SM_DemoRoomU2;

			}

			if (bOpenRoof)
			{
				TempMesh = SM_DemoRoomL;
			}

			MeshComp->SetStaticMesh(TempMesh);
		}

		// Set Color
		if (bSwitchColor)
		{
			if (auto MI = MeshComp->CreateDynamicMaterialInstance(0, M_DemoRoomTiles_Inst))
			{
				MI->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.03f, 0.03f, 0.03f, 1));
				MI->SetScalarParameterValue(TEXT("RoughNess"), 0.5f);
			}
		}
	}
}

void ADemoRoom::AddBackWall(float InOffset, int32 InRoomSize)
{
	if (UStaticMeshComponent * MeshComponent = NewObject<UStaticMeshComponent>(this))
	{
		// Add Wall mesh
		FVector InLoc = TrimWidth;
		InLoc.X /= InOffset;

		FVector InScale = (SectionWidth * InRoomSize) / FVector(1750.0f, 1.0f, 1.0f);
		InScale.Y = 0;
		InScale.Z = 0;

		FTransform TempTransform = UKismetMathLibrary::MakeTransform(InLoc, FRotator::ZeroRotator, InScale);
		MeshComponent->SetRelativeTransform(TempTransform);

		// Different wall if double height room
		if (bDoubleHeight)
		{
			MeshComponent->SetStaticMesh(SM_DemoRoomBackWall);
		}
		else
		{
			MeshComponent->SetStaticMesh(SM_DemoRoomBackWall2);
		}
	}
}

void ADemoRoom::AddClamp(float InOffset, int32 InRoomSize, int32 InIndex)
{
	UStaticMesh* InMesh = nullptr;
	if (bDoubleHeight)
	{
		InMesh = SM_DemoRoomClamp2;
	}
	else
	{
		InMesh = SM_DemoRoomClamp;
	}

	if (UStaticMeshComponent * MeshComponent = NewObject<UStaticMeshComponent>())
	{
		FVector InLoc = FVector(InOffset, 0, 0);
		FVector InScale = FVector::OneVector;
		FTransform TempTransform = UKismetMathLibrary::MakeTransform(InLoc, FRotator::ZeroRotator,
			InScale);

		MeshComponent->SetRelativeTransform(TempTransform);
		MeshComponent->SetStaticMesh(InMesh);
	}

	if (bOpenBack == false)
	{
		int32 ClampedNum = FMath::Clamp(NumberofRooms - 1, 0, 30);
		if (ClampedNum == InIndex)
		{
			if (UStaticMeshComponent * MeshComponent = NewObject<UStaticMeshComponent>())
			{
				FVector InLoc = (TrimWidth + SectionWidth * InIndex);
				InLoc.X += InOffset;

				FVector InScale = FVector::OneVector;
				FTransform InTransform = UKismetMathLibrary::MakeTransform(InLoc, FRotator::ZeroRotator,
					InScale);

				MeshComponent->SetRelativeTransform(InTransform);
				MeshComponent->SetStaticMesh(InMesh);
			}
		}
	}
}

void ADemoRoom::AddTrim(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex)
{
	UStaticMesh* InMesh = nullptr;
	if (bDoubleHeight)
	{
		InMesh = SM_DemoRoomTrim2;
	}
	else
	{
		InMesh = SM_DemoRoomTrim;
	}

	FVector InLocation(InOffset, 0, 0);
	FTransform InTransform = UKismetMathLibrary::MakeTransform(InLocation, FRotator::ZeroRotator, InScale);
	if (UStaticMeshComponent * MeshComponent = NewObject<UStaticMeshComponent>())
	{
		MeshComponent->SetRelativeTransform(InTransform);
		MeshComponent->SetStaticMesh(InMesh);
	}

	// Add and set transformation for last mesh
	if (bOpenBack)
	{
		return;
	}

	int32 Number = FMath::Clamp(NumberofRooms - 1, 0, 31);
	if (InIndex != Number)
	{
		return;
	}

	FVector InLocation2(TrimWidth + (SectionWidth * InRoomSize));
	InLocation2.X += InOffset;
	FTransform InTransform2 = UKismetMathLibrary::MakeTransform(InLocation2, FRotator::ZeroRotator, InScale);
	if (UStaticMeshComponent * MeshComponent = NewObject<UStaticMeshComponent>())
	{
		MeshComponent->SetRelativeTransform(InTransform2);
		MeshComponent->SetStaticMesh(InMesh);
	}
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
	if (bLights == false)
	{
		return;
	}

	FVector TempVector = SectionWidth;
	float TempX = TempVector.X * InRoomSize;
	float CalculatedX = TempX / 2000.0;
	int32 Loop = FMath::Clamp((FMath::FloorToInt(CalculatedX)), 1, 50);
	for (int I = 1; I < Loop; ++I)
	{
		float MultiedValue = 2000.0 * I;
	
		float Fraction = FMath::Frac(TempX);
		float SelectedValue = InRoomSize <= 1 ? Fraction : -Fraction;
		SelectedValue *= TempX;

		float CalculatedValue = InOffset + (MultiedValue - SelectedValue) - ((CalculatedX / TempX / 2) - 300);
	}

}

void ADemoRoom::AddOpenTrim(FVector InScale, float InOffset, int32 InRoomSize, int32 InIndex)
{

}

void ADemoRoom::AddRoomNames(float InOffset, int32 InRoomSize, int32 InIndex)
{

}
