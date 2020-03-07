#include "DisplayedItem.h"

#include "Components/PrimitiveComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Items/ObjectItems/ItemBase.h"

#include "DCSLib.h"

void ADisplayedItem::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* CPrimary = GetPrimaryComponent();
	if (CPrimary == nullptr)
	{
		return;
	}

	CPrimary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UPrimitiveComponent* ADisplayedItem::GetPrimaryComponent() const
{
	UPrimitiveComponent* Ret = UDCSLib::GetComponent<UStaticMeshComponent>(*this);
	if (Ret)
	{
		return Ret;
	}

	Ret = UDCSLib::GetComponent<USkeletalMeshComponent>(*this);
	if (Ret)
	{
		return Ret;
	}

	Ret = UDCSLib::GetComponent<UParticleSystemComponent>(*this);
	if (Ret)
	{
		return Ret;
	}
	
	return nullptr;
}

FName ADisplayedItem::GetAttachmentSocket() const
{
	return AttachmentSocket;
}

bool ADisplayedItem::Attach()
{
	UPrimitiveComponent* CPrimary = GetPrimaryComponent();
	if (CPrimary == nullptr)
	{
		return false;
	}

	if (auto Character = Cast<ACharacter>(GetOwner()))
	{
		CPrimary->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachmentSocket);
	}

	return true;
}

void ADisplayedItem::SimulatePhysics()
{
	auto CPrimary = GetPrimaryComponent();
	CPrimary->SetCollisionProfileName(FName(TEXT("Ragdoll")));
	CPrimary->SetSimulatePhysics(true);
}

