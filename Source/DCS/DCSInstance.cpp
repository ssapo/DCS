#include "DCSInstance.h"
#include "DCS.h"
#include "WidgetSystem.h"

void UDCSInstance::Init()
{
	Super::Init();
	DLOG_S(Log);

	WidgetSystem = NewObject<UWidgetSystem>(this);
}

void UDCSInstance::OnStart()
{
	Super::OnStart();
	DLOG_S(Log);

	WidgetSystem->StartSystem();
}

void UDCSInstance::Shutdown()
{
	Super::Shutdown();

	DLOG_S(Log);

	WidgetSystem->FinishSystem();
}

void UDCSInstance::StartGameInstance()
{
	Super::StartGameInstance();

	DLOG_S(Log);
}

