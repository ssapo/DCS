#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Structs.h"
#include "ANS_SetDisplayedItemVisibility.generated.h"

UCLASS()
class DCS_API UANS_SetDisplayedItemVisibility : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_SetDisplayedItemVisibility();

private:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere)
		EItem Item;

	UPROPERTY(EditAnywhere)
		bool bHasToBeInCombat;

	UPROPERTY(EditAnywhere)
		bool bHideThenShow;
};
