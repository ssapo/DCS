#include "AN_ToggleCombat.h"

#include "DCSLib.h"

#include "Components/EquipmentComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UAN_ToggleCombat::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UEquipmentComponent* CEquip = UDCSLib::GetComponent<UEquipmentComponent>(MeshComp->GetOwner());
	if (CEquip)
	{
		CEquip->ToggleCombat();
	}
}

FString UAN_ToggleCombat::GetNotifyName_Implementation() const
{
	return TEXT("ToggleCombat");
}

