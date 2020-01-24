#include "ANS_HitBox.h"

#include "DCSLib.h"

#include "Components/CollisionHandlerComponent.h"

UANS_HitBox::UANS_HitBox()
{

}

void UANS_HitBox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto CCollision = UDCSLib::GetComponent<UCollisionHandlerComponent>(MeshComp->GetOwner());
	if (CCollision == nullptr)
	{
		return;
	}

	CCollision->ActivateCollision(PartType);
}

void UANS_HitBox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto CCollision = UDCSLib::GetComponent<UCollisionHandlerComponent>(MeshComp->GetOwner());
	if (CCollision == nullptr)
	{
		return;
	}

	CCollision->DeActivateCollision();
}

FString UANS_HitBox::GetNotifyName_Implementation() const
{
	return TEXT("HitBox");
}
