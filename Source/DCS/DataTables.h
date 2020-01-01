#pragma once
#include "Engine/DataTable.h"
#include "DataTables.generated.h"

class UDCSWidget;

UENUM()
enum class EWidgetID
{
	None = 0,
	InGame,
	KeyBindings,
};

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
