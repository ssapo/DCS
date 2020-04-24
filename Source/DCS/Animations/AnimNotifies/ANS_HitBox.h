#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Structs.h"
#include "ANS_HitBox.generated.h"

UCLASS()
class DCS_API UANS_HitBox : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UANS_HitBox();

private:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const override;

private:
	ECollisionPart PartType;

};
