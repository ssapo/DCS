#include "ANS_InputBuffer.h"

#include "Components/InputBufferComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"

#include "DCSLib.h"

void UANS_InputBuffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	auto CInputBuffer = UDCSLib::GetComponent<UInputBufferComponent>(MeshComp->GetOwner());
	if (CInputBuffer)
	{
		CInputBuffer->OpenInputBuffer();
	}
}

void UANS_InputBuffer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto CInputBuffer = UDCSLib::GetComponent<UInputBufferComponent>(MeshComp->GetOwner());
	if (CInputBuffer)
	{
		CInputBuffer->CloseInputBuffer();
	}
}

FString UANS_InputBuffer::GetNotifyName_Implementation() const
{
	return TEXT("InputBuffer");
}
