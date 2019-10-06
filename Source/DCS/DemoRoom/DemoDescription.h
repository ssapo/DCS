// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemoDescription.generated.h"

class UTextRenderComponent;
class UFont;

UCLASS(Blueprintable)
class DCS_API ADemoDescription : public AActor
{
	GENERATED_BODY()
	
public:	
	ADemoDescription();
	
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void CtorComponents();
	void Initialize();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UTextRenderComponent* TRTitle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UTextRenderComponent* TRDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* SMBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* SMBaseFlatWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FLinearColor StandColor;
};
