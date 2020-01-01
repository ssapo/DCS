#include "DCSInstance.h"
#include "DCS.h"

void UDCSInstance::Init()
{
	Super::Init();

	LOG_S(Log);
}

void UDCSInstance::Shutdown()
{
	Super::Shutdown();

	LOG_S(Log);
}

void UDCSInstance::StartGameInstance()
{
	Super::StartGameInstance();

	LOG_S(Log);
}