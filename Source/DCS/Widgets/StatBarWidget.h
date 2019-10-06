// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DCSWidget.h"
#include "StatBarWidget.generated.h"

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

};
