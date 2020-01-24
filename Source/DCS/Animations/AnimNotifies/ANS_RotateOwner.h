#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_RotateOwner.generated.h"

UCLASS()
class DCS_API UANS_RotateOwner : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_RotateOwner();

private:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY(EditAnywhere)
		float MaxDegreesPerSecond;

	UPROPERTY(EditAnywhere)
		float RotateTime;
};
