#include "ANS_Acitivity.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StateManagerComponent.h"
#include "Animation/AnimSequenceBase.h"

#include "DCSLib.h"

UANS_Acitivity::UANS_Acitivity()
{
	Activity = EActivity::IsImmortal;
}

void UANS_Acitivity::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	auto CStateManager = UDCSLib::GetComponent<UStateManagerComponent>(MeshComp->GetOwner());
	if (CStateManager)
	{
		CStateManager->SetActivity(Activity, true);
	}

	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UANS_Acitivity::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto CStateManager = UDCSLib::GetComponent<UStateManagerComponent>(MeshComp->GetOwner());
	if (CStateManager)
	{
		CStateManager->SetActivity(Activity, false);
	}

	Super::NotifyEnd(MeshComp, Animation);
}

FString UANS_Acitivity::GetNotifyName_Implementation() const
{
	return UDCSLib::GetStringAsEnum(TEXT("EActivity"), static_cast<int32>(Activity));
}
