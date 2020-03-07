#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "NameTypes.h"
#include "WeakObjectPtrTemplates.h"
#include "DisplayedItem.generated.h"

class UEquipmentComponent;

UCLASS()
class DCS_API ADisplayedItem : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual FName GetAttachmentSocket() const;
	UPrimitiveComponent* GetPrimaryComponent() const;

	bool Attach();
	void SimulatePhysics();
	
protected:
	UPROPERTY(Transient)
		TWeakObjectPtr<UEquipmentComponent> WP_Equip;

	UPROPERTY(EditAnywhere)
		FName AttachmentSocket;

	UPROPERTY(EditAnywhere)
		int32 SlotIndex;

	UPROPERTY(EditAnywhere)
		EItem ItemType;
};
