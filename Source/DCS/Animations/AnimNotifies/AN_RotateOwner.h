#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_RotateOwner.generated.h"

UCLASS()
class DCS_API UAN_RotateOwner : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAN_RotateOwner();

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere)
		float MaxPossibleRotation;

	UPROPERTY(EditAnywhere)
		float MaxDegreesPerSecond;
};
