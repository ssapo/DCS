#pragma once
#include "Engine/DataTable.h"
#include "Enumerations.h"
#include "DataTables.generated.h"

class UDCSWidget;
class UAnimMontage;

USTRUCT(BlueprintType)
struct DCS_API FWidgetDefinitnionData : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere)
		EWidgetID WidgetType;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDCSWidget> BP_Widget;

	UPROPERTY(EditAnywhere)
		int32 ZOrder;
};


USTRUCT(BlueprintType)
struct DCS_API FMontageActionData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMontageActionData()
		: Action(EMontage::None)
		, Montages()
	{}

	UPROPERTY(EditAnywhere)
		EMontage Action;

	UPROPERTY(EditAnywhere)
		TArray<UAnimMontage*> Montages;
};
