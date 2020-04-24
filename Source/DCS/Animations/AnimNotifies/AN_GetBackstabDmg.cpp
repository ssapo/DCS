#include "AN_GetBackstabDmg.h"

#include "DCSLib.h"

#include "Components/EffectsComponent.h"

#include "Structs.h"

void UAN_GetBackstabDmg::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	auto OnwerActor = MeshComp->GetOwner();
	UEffectsComponent* CEffects = UDCSLib::GetComponent<UEffectsComponent>(OnwerActor);
	if (CEffects == nullptr)
	{
		return;
	}

	// TODO: fill function
}

FString UAN_GetBackstabDmg::GetNotifyName_Implementation() const
{
	return TEXT("GetBackstabDmg");
}
