// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoDisplay.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

void ADemoDisplay::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Initialize();
}

void ADemoDisplay::Initialize()
{
	FixLegacyScale();

	switch (DisplayType)
	{
	case EDemoDisplay::Round:
		DisplayRound();
		break;

	case EDemoDisplay::SquareL:
		DisplaySquareL();
		break;

	case EDemoDisplay::RoomL:
		DisplayRoomL();
		break;

	case EDemoDisplay::DescriptionOnly:
		DisplayDescriptionOnly();
		break;

	case EDemoDisplay::SquareLFlatWall:
		DisplaySquareLFlatWall();
		break;

	default:
		break;
	}
}


void ADemoDisplay::DisplayRound()
{
	FTransform TempTransform = ScaleTransformLocation(UKismetMathLibrary::MakeTransform(
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		MeshScale));

	FTransform LineTransform = ScaleTransformLocation(UKismetMathLibrary::MakeTransform(
		FVector(0, 351, 15),
		FRotator(0, 0, 0),
		MeshScale));

	CreateRound_BP(TempTransform, LineTransform);
	AddDescription(LineTransform);
}

void ADemoDisplay::DisplaySquareL()
{
	FTransform TempTransform = ScaleTransformLocation(UKismetMathLibrary::MakeTransform(
		FVector(0, 0, -25),
		FRotator(0, 0, 0),
		MeshScale));

	FTransform LineTransform = ScaleTransformLocation(UKismetMathLibrary::MakeTransform(
		FVector(0, 252, 15),
		FRotator(0, 0, 0),
		MeshScale));

	CreateSquareL_BP(TempTransform, LineTransform);

	FTransform NumberTransform = ScaleTransformLocationScale(UKismetMathLibrary::MakeTransform(
		FVector(-550, -705, 780),
		FRotator(0, 90, 0),
		FVector::OneVector));

	if (Number.IsEmpty() == false)
	{
		if (auto TRNumber = AddTextRenderDisplay_BP())
		{
			TRNumber->SetRelativeTransform(NumberTransform);
			TRNumber->SetMaterial(0, MIDefaultText);
			TRNumber->SetText(Number);
		}
	}

	AddDescription(LineTransform);
}

void ADemoDisplay::DisplayRoomL()
{
	FTransform TempTransform = ScaleTransformLocation(UKismetMathLibrary::MakeTransform(
		FVector(0, 0, -25),
		FRotator::ZeroRotator,
		MeshScale));

	CreateRoomL_BP(TempTransform);

	FTransform NumberTransform = ScaleTransformLocationScale(UKismetMathLibrary::MakeTransform(
		FVector(-550, -705, 780),
		FRotator(0, 90, 0),
		FVector::OneVector));

	if (Number.IsEmpty() == false)
	{
		if (auto TRNumber = AddTextRenderDisplay_BP())
		{
			TRNumber->SetRelativeTransform(NumberTransform);
			TRNumber->SetMaterial(0, MIDefaultText);
			TRNumber->SetText(Number);
		}
	}
}

void ADemoDisplay::DisplayDescriptionOnly()
{
	FTransform TempTransform = ScaleTransformLocation(UKismetMathLibrary::MakeTransform(
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		MeshScale));

	CreateDescriptionOnly_BP(TempTransform);
	AddDescription(TempTransform);
}

void ADemoDisplay::DisplaySquareLFlatWall()
{
	FTransform TempTransform = ScaleTransformLocation(UKismetMathLibrary::MakeTransform(
		FVector(0, 0, -25),
		FRotator::ZeroRotator,
		MeshScale));

	CreateSquareLFlatWall_BP(TempTransform);

	FTransform NumberTransform = ScaleTransformLocationScale(UKismetMathLibrary::MakeTransform(
		FVector(-530, -222, 795),
		FRotator(0, 90, 0),
		FVector(0.75f, 0.75f, 0.75f)));

	if (Number.IsEmpty() == false)
	{
		if (auto TRNumber = AddTextRenderDisplay_BP())
		{
			TRNumber->SetRelativeTransform(NumberTransform);
			TRNumber->SetMaterial(0, MIDefaultText);
			TRNumber->SetText(Number);
		}
	}

	DisplaySquareL();
}

void ADemoDisplay::AddDescription(const FTransform& InTransform)
{
	Add2ndLineDescription(Add1stLineDescription(InTransform));
}

FTransform ADemoDisplay::Add1stLineDescription(const FTransform& InTransform)
{
	if (MIDefaultText == nullptr)
	{
		return FTransform::Identity;
	}

	if (Description.IsEmpty())
	{
		return FTransform::Identity;
	}

	auto TRTemp = AddTextRenderDescription_BP();
	if (TRTemp == nullptr)
	{
		return FTransform::Identity;
	}

	/*		Fix for legacy content that has wrong scale.	 */
	FTransform ScaledInTransform = UKismetMathLibrary::MakeTransform(
		InTransform.GetLocation(),
		InTransform.GetRotation().Rotator(),
		(InTransform.GetScale3D() / MeshScale) * BeUsedScaleTransform
	);

	FVector Loc1stLine = PositionBias1stLine * BeUsedScaleTransform;
	FVector Loc2ndLine = PositionBias2ndLine * BeUsedScaleTransform;
	//////////////////////////////////////////////////////////

	FVector ScaledLoc;
	FRotator ScaledRot;
	FVector ScaledScale;
	UKismetMathLibrary::BreakTransform(ScaledInTransform, ScaledLoc, ScaledRot, ScaledScale);

	/*				Add Text and Set Positions.				 */
	FVector Loc1PlLoc = ScaledLoc + Loc1stLine;
	FVector Loc2ndSelectedLoc = UKismetMathLibrary::SelectVector(
		FVector::ZeroVector,
		FVector(0, -5, 10),
		Description2ndLine.IsEmpty());
	Loc2ndSelectedLoc *= BeUsedScaleTransform;

	FVector Loc1PlLoc2 = Loc1PlLoc + Loc2ndSelectedLoc;
	FRotator NewRot = ScaledRot;
	NewRot.Yaw += 90;
	NewRot.Pitch = 33;

	FTransform ResultTransform = UKismetMathLibrary::MakeTransform(
		Loc1PlLoc2,
		NewRot,
		ScaledScale);

	TRTemp->SetRelativeTransform(ResultTransform);
	TRTemp->SetMaterial(0, MIDefaultText);
	TRTemp->SetText(Description);
	TRTemp->SetXScale(BeUsedTextScale);
	TRTemp->SetYScale(BeUsedTextScale);

	/*		MakeTransform return value for 		 */
	FVector Loc2PlLoc1 = Loc1PlLoc + Loc2ndLine;
	return UKismetMathLibrary::MakeTransform(
		Loc2PlLoc1,
		NewRot,
		ScaledInTransform.GetScale3D());
}

void ADemoDisplay::Add2ndLineDescription(const FTransform& InTransform)
{
	if (MIDefaultText == nullptr)
	{
		return;
	}

	if (Description2ndLine.IsEmpty())
	{
		return;
	}
	
	auto TRTemp = AddTextRenderDescription_BP();
	if (TRTemp == nullptr)
	{
		return;
	}

	TRTemp->SetRelativeTransform(InTransform);
	TRTemp->SetMaterial(0, MIDefaultText);
	TRTemp->SetText(Description2ndLine);
	TRTemp->SetXScale(BeUsedTextScale);
	TRTemp->SetYScale(BeUsedTextScale);
}

void ADemoDisplay::FixLegacyScale()
{
	if (!bIsFixScale)
	{
		return;
	}

	BeUsedScaleTransform = 0.5f;
	MeshScale = FVector::OneVector;
}


FTransform ADemoDisplay::ScaleTransformLocation(const FTransform& InTransform)
{
	return UKismetMathLibrary::MakeTransform(
		InTransform.GetLocation() * BeUsedScaleTransform,
		InTransform.GetRotation().Rotator(),
		InTransform.GetScale3D());
}

FTransform ADemoDisplay::ScaleTransformLocationScale(const FTransform& InTransform)
{
	return UKismetMathLibrary::MakeTransform(
		InTransform.GetLocation() * BeUsedScaleTransform,
		InTransform.GetRotation().Rotator(),
		InTransform.GetScale3D() * BeUsedScaleTransform);
}