#include "ANS_RotateOwner.h"
#include "DCSLib.h"

#include "Components/RotatingComponent.h"

UANS_RotateOwner::UANS_RotateOwner()
{
	MaxDegreesPerSecond = 720.0f;

	RotateTime = 10.0f;
}

void UANS_RotateOwner::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto CRotating = UDCSLib::GetComponent<URotatingComponent>(MeshComp->GetOwner());
	if (CRotating == nullptr)
	{
		return;
	}

	CRotating->StartRotatingWithTime(RotateTime, MaxDegreesPerSecond);
}

void UANS_RotateOwner::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto CRotating = UDCSLib::GetComponent<URotatingComponent>(MeshComp->GetOwner());
	if (CRotating == nullptr)
	{
		return;
	}

	CRotating->StopRotating();
}
