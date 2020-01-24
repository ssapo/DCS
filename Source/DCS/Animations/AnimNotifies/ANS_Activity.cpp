#include "ANS_Activity.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StateManagerComponent.h"
#include "Animation/AnimSequenceBase.h"

#include "DCSLib.h"

UANS_Activity::UANS_Activity()
{
	Activity = EActivity::IsImmortal;
}

void UANS_Activity::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	auto CStateManager = UDCSLib::GetComponent<UStateManagerComponent>(MeshComp->GetOwner());
	if (CStateManager)
	{
		CStateManager->SetActivity(Activity, true);
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UANS_Activity::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto CStateManager = UDCSLib::GetComponent<UStateManagerComponent>(MeshComp->GetOwner());
	if (CStateManager)
	{
		CStateManager->SetActivity(Activity, false);
	}

	Super::NotifyEnd(MeshComp, Animation);
}

FString UANS_Activity::GetNotifyName_Implementation() const
{
	return UDCSLib::GetStringAsEnum(TEXT("EActivity"), static_cast<int32>(Activity));
}
