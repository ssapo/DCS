#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ToggleCombat.generated.h"

UCLASS()
class DCS_API UAN_ToggleCombat : public UAnimNotify
{
	GENERATED_BODY()

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const override;
};
