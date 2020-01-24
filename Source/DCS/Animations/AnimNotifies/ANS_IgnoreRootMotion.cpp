#include "ANS_IgnoreRootMotion.h"

#include "Animation/AnimEnums.h"

#include "Components/SkeletalMeshComponent.h"

UANS_IgnoreRootMotion::UANS_IgnoreRootMotion()
{
}

void UANS_IgnoreRootMotion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	MeshComp->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
}

void UANS_IgnoreRootMotion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	MeshComp->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
}

FString UANS_IgnoreRootMotion::GetNotifyName_Implementation() const
{
	return TEXT("IgnoreRM");
}
