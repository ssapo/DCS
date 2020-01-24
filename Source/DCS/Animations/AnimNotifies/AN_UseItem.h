#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Structs.h"
#include "AN_UseItem.generated.h"

UCLASS()
class DCS_API UAN_UseItem : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere)
		EItem Item;
};
