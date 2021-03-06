// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "DCSWidget.h"
#include "WeakObjectPtrTemplates.h"
#include "StatBarWidget.generated.h"

class UProgressBar;
class UExtendedStatComponent;
class USizeBox;

/**
 * 
 */
UCLASS()
class DCS_API UStatBarWidget : public UDCSWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void SetStatBar(UExtendedStatComponent* InComponent);

	void OnValueChanged(float InValue, float MaxValue);

	void UpdatePercent(float InPercent);
	void UpdateWidth(float InMaxValue);

private:
	TWeakObjectPtr<UExtendedStatComponent> WP_CStat;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* StatProgressBar;

	UPROPERTY(meta = (BindWidget))
		USizeBox* StatSizeBox;

	UPROPERTY(EditAnywhere)
		FLinearColor FillColor;

	UPROPERTY(EditAnywhere)
		EStat ExtendedStatType;
};
