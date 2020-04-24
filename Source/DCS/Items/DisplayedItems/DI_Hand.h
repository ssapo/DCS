
#pragma once

#include "CoreMinimal.h"
#include "Items/DisplayedItems/DisplayedItem.h"
#include "DI_Hand.generated.h"

UCLASS()
class DCS_API ADI_Hand : public ADisplayedItem
{
	GENERATED_BODY()
	
public:	
	ADI_Hand();
	
protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnSlotHiddenChanged(EItem Slot, int32 SlotIndex, const FStoredItem& ActiveItem, bool IsHidden);
	virtual FName GetAttachmentSocket() const override;

protected:
	UPROPERTY(EditAnywhere)
		FName HandAttachmentSocket;
};
