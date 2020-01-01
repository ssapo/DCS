#include "DCSInstance.h"
#include "DCS.h"

void UDCSInstance::Init()
{
	Super::Init();

	DLOG_S(Log);
}

void UDCSInstance::Shutdown()
{
	Super::Shutdown();

	DLOG_S(Log);
}

void UDCSInstance::StartGameInstance()
{
	Super::StartGameInstance();

	DLOG_S(Log);
}