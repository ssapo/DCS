#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_IgnoreRootMotion.generated.h"

UCLASS()
class DCS_API UANS_IgnoreRootMotion : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UANS_IgnoreRootMotion();

private:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const override;

};
