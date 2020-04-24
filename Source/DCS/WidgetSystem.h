#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Map.h"
#include "WidgetSystem.generated.h"

class UDataTable;
class UDCSWidget;
enum class EWidgetID;
struct FWidgetDefinitnionData;

UCLASS()
class DCS_API UWidgetSystem : public UObject
{
	GENERATED_BODY()
	
public:
	static UWidgetSystem const* Get();

	void StartSystem();
	void FinishSystem();

	UDCSWidget* ShowWidget(EWidgetID InType) const;

private:
	bool LoadTable();

private:
	static TWeakObjectPtr<UWidgetSystem> This;

	TMap<EWidgetID, const FWidgetDefinitnionData*> WidgetClasses;
};
