// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatBarsWidget.h"
#include "StatBarWidget.h"

void UAIStatBarsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Stamina->SetVisibility(ESlateVisibility::Collapsed);
}
