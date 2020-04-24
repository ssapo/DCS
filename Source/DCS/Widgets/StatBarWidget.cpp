// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBarWidget.h"
#include "DCSLib.h"
#include "ProgressBar.h"
#include "SizeBox.h"
#include "GameFramework/PlayerController.h"
#include "Components/ExtendedStatComponent.h"
#include "CanvasPanelSlot.h"

void UStatBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	StatProgressBar->SetFillColorAndOpacity(FillColor);
}

void UStatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetStatBar(UDCSLib::GetComponent<UExtendedStatComponent>(GetOwningPlayerPawn()));
}

void UStatBarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (WP_CStat.IsValid())
	{
		WP_CStat->OnValueChanged().RemoveAll(this);
	}
}

void UStatBarWidget::SetStatBar(UExtendedStatComponent* InComponent)
{
	WP_CStat = InComponent;
	if (WP_CStat.IsValid() == false)
	{
		return;
	}
	
	OnValueChanged(WP_CStat->GetCurrentValue(), WP_CStat->GetMaxValue());
	WP_CStat->OnValueChanged().AddUObject(this, &UStatBarWidget::OnValueChanged);
}

void UStatBarWidget::OnValueChanged(float InValue, float MaxValue)
{
	float Percent = FMath::Clamp<float>(InValue / MaxValue, 0, 1);
	UpdatePercent(Percent);
	UpdateWidth(MaxValue);
}

void UStatBarWidget::UpdatePercent(float InPercent)
{
	StatProgressBar->SetPercent(InPercent);
}

void UStatBarWidget::UpdateWidth(float InMaxValue)
{
	if (APlayerController* Player = GetOwningPlayer())
	{
		float MaxClampledValue = 0;
		float InputValue = 0;

		if (UCanvasPanelSlot * Canvas = UDCSLib::SlotAsCanvasSlot(StatSizeBox))
		{
			float PositionX = Canvas->GetPosition().X * 2;
			float SizeX = UDCSLib::GetViewportSize(this).X;
			MaxClampledValue = SizeX - PositionX;
		}
		else
		{
			MaxClampledValue = UDCSLib::GetViewportSize(this).X - 200;
		}
		
		if (MaxClampledValue >= 0)
		{
			InputValue = MaxClampledValue;
		}
		else
		{
			InputValue = 1000.0f;
		}

		InputValue = FMath::Clamp<float>(InMaxValue * 3, 200, InputValue);
		StatSizeBox->SetWidthOverride(InputValue);
	}
}
