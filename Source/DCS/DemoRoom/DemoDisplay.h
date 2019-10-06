// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "DemoDisplay.generated.h"

class USceneComponent;
class UTextRenderComponent;
class UStaticMeshComponent;
class UMaterialInterface;

UCLASS(Blueprintable)
class DCS_API ADemoDisplay : public AActor
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void CreateRound_BP(const FTransform& InTrnasform1, const FTransform& InTrnasform2);
	UFUNCTION(BlueprintImplementableEvent)
		void CreateSquareL_BP(const FTransform& InTrnasform1, const FTransform& InTrnasform2);
	UFUNCTION(BlueprintImplementableEvent)
		void CreateRoomL_BP(const FTransform& InTrnasform);
	UFUNCTION(BlueprintImplementableEvent)
		void CreateDescriptionOnly_BP(const FTransform& InTrnasform);
	UFUNCTION(BlueprintImplementableEvent)
		void CreateSquareLFlatWall_BP(const FTransform& InTrnasform);
	UFUNCTION(BlueprintImplementableEvent)
		UTextRenderComponent* AddTextRenderDisplay_BP();
	UFUNCTION(BlueprintImplementableEvent)
		UTextRenderComponent* AddTextRenderDescription_BP();

private:
	void Initialize();
	void DisplayRound();
	void DisplaySquareL();
	void DisplayRoomL();
	void DisplayDescriptionOnly();
	void DisplaySquareLFlatWall();

	void AddDescription(const FTransform& InTransform);
	FTransform Add1stLineDescription(const FTransform& InTransform);
	FTransform ScaleTransformLocation(const FTransform& InTransform);
	FTransform ScaleTransformLocationScale(const FTransform& InTransform);
	void Add2ndLineDescription(const FTransform& InTransform);
	void FixLegacyScale();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UMaterialInterface* MIDefaultText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description", meta = (AllowPrivateAccess = true))
		FText Number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description", meta = (AllowPrivateAccess = true))
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description", meta = (AllowPrivateAccess = true))
		FText Description2ndLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector", meta = (AllowPrivateAccess = true))
		FLinearColor BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector", meta = (AllowPrivateAccess = true))
		FVector PositionBias1stLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector", meta = (AllowPrivateAccess = true))
		FVector PositionBias2ndLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description", meta = (AllowPrivateAccess = true))
		EDemoDisplay DisplayType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description", meta = (AllowPrivateAccess = true))
		float BeUsedScaleTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Description", meta = (AllowPrivateAccess = true))
		float BeUsedTextScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector", meta = (AllowPrivateAccess = true))
		FVector MeshScale;

	TWeakObjectPtr<USceneComponent> Scene1;
	bool bIsFixScale;
};
