#include "MontageManagerComponent.h"
#include "DCSLib.h"
#include "Defines.h"
#include "Interfaces/IsMontageManager.h"
#include "Engine/DataTable.h"

// start public:
UMontageManagerComponent::UMontageManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LastRequestedAction = EMontage::None;
}

int32 UMontageManagerComponent::GetMontageActionLastIndex(EMontage InType) const
{
	FMontageActionData Data = GetMontage(LastRequestedAction);
	return Data.Montages.Num() - 1;
}

UAnimMontage* UMontageManagerComponent::GetMontageForAction(EMontage InType, int32 Index)
{
	LastRequestedAction = InType;

	FMontageActionData Data = GetMontage(LastRequestedAction);
	if (Data.Montages.IsValidIndex(Index))
	{
		return Data.Montages[Index];
	}

	return nullptr;
}

EMontage UMontageManagerComponent::GetLastRequestedAction() const
{
	return LastRequestedAction;
}

// end public:

// start protected:
void UMontageManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	check(UDCSLib::IsInterface(GetOwner(), UIsMontageManager::StaticClass()));

	IMontageManager = Cast<IIsMontageManager>(GetOwner());
}

void UMontageManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
// end protected:

// start private:
FMontageActionData UMontageManagerComponent::GetMontage(EMontage InAction) const
{
	FMontageActionData RetVal;
	UDataTable* Montages = IMontageManager->GetMontages(InAction);
	const TMap<FName, uint8*>& MontagesMap = Montages->GetRowMap();
	FName Name = *UDCSLib::GetStringAsEnum(TEXT("EMontage"), static_cast<int32>(InAction));
	if (MontagesMap.Contains(Name))
	{
		RetVal = *(FMontageActionData*)(MontagesMap[Name]);
	}

	return RetVal;
}
// end private:
