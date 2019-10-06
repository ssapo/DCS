// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoDescription.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"

ADemoDescription::ADemoDescription()
{
	CtorComponents();
}

void ADemoDescription::CtorComponents()
{
	auto Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	ensure(Scene);
	RootComponent = Scene;

	SMBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMBase"));
	ensure(SMBase);
	SMBase->SetupAttachment(RootComponent);

	SMBaseFlatWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMBaseFlatWall"));
	ensure(SMBaseFlatWall);
	SMBaseFlatWall->SetupAttachment(RootComponent);

	TRTitle = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TitleText"));
	ensure(TRTitle);
	TRTitle->SetupAttachment(RootComponent);

	TRDescription = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DescriptionText"));
	ensure(TRDescription);
	TRDescription->SetupAttachment(RootComponent);
}

void ADemoDescription::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Initialize();
}

void ADemoDescription::Initialize()
{
	if (SMBase)
	{
		if (auto MIBase = SMBase->CreateDynamicMaterialInstance(3))
		{
			MIBase->SetVectorParameterValue(TEXT("Color"), StandColor);
		}
	}

	if (SMBaseFlatWall)
	{
		if (auto MIBaseFlatWall = SMBaseFlatWall->CreateDynamicMaterialInstance(0))
		{
			MIBaseFlatWall->SetVectorParameterValue(TEXT("Color"), StandColor);
		}
	}
}