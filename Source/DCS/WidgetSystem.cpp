#include "WidgetSystem.h"
#include "Engine/DataTable.h"
#include "DataTables.h"
#include "Widgets/DCSWidget.h"

TWeakObjectPtr<UWidgetSystem> UWidgetSystem::This;

UWidgetSystem const* UWidgetSystem::Get()
{
	return This.IsValid() ? This.Get() : nullptr;
}

void UWidgetSystem::StartSystem()
{
	This = this;

	bool bSuccess = LoadTable();
	if (bSuccess == false)
	{
		ensureMsgf(bSuccess, TEXT("failed LoadTable"));
	}
}

void UWidgetSystem::FinishSystem()
{
	This.Reset();
}

UDCSWidget* UWidgetSystem::ShowWidget(EWidgetID InType) const
{
	if (WidgetClasses.Contains(InType) == false)
	{
		ensureMsgf(WidgetClasses.Contains(InType), TEXT("expression should not be false."));
	}
	
	const FWidgetDefinitnionData* Data = WidgetClasses[InType];
	auto NewWidget = CreateWidget<UDCSWidget>(GetWorld(), Data->BP_Widget);
	if (NewWidget == nullptr)
	{
		ensureMsgf(NewWidget, TEXT("expression should not be null."));
		return nullptr;
	}

	NewWidget->AddToViewport(Data->ZOrder);

	return NewWidget;
}

bool UWidgetSystem::LoadTable()
{
	auto WidgetTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL,
		TEXT("/Game/DynamicCombatSystem/DataTables/WidgetDefinition.WidgetDefinition")));
	if (WidgetTable == false)
	{
		return false;
	}

	TArray<FWidgetDefinitnionData*> WidgetDefinitions;
	WidgetTable->GetAllRows<FWidgetDefinitnionData>(TEXT(""), WidgetDefinitions);

	if (WidgetDefinitions.Num() <= 0)
	{
		return false;
	}

	for (const auto& Each : WidgetDefinitions)
	{
		WidgetClasses.Add(Each->WidgetType, Each);
	}

	return WidgetClasses.Num() > 0;
}

