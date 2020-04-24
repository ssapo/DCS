#include "AN_RotateOwner.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/RotatingComponent.h"

#include "Animation/AnimSequenceBase.h"

#include "DCSLib.h"

UAN_RotateOwner::UAN_RotateOwner()
{
	MaxPossibleRotation = 720.0f;
	MaxDegreesPerSecond = 180.0f;
}

void UAN_RotateOwner::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto CRotating = UDCSLib::GetComponent<URotatingComponent>(MeshComp->GetOwner());
	if (CRotating)
	{
		CRotating->StartRotatingWithLimit(MaxPossibleRotation, MaxDegreesPerSecond);
	}

	Super::Notify(MeshComp, Animation);
}
