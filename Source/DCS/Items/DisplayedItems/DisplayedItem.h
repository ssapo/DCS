#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "NameTypes.h"
#include "DisplayedItem.generated.h"

class UEquipmentComponent;

UCLASS()
class DCS_API ADisplayedItem : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPrimitiveComponent* GetPrimaryComponent() const;
	bool Attach();
	void SimulatePhysics();
	
private:
	UEquipmentComponent* CEquip;

	FName AttachmentSocket;
	int32 SLotIndex;
	EItem ItemType;
};
