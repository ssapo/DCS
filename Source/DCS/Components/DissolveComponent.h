#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DissolveComponent.generated.h"

class UMaterialInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DCS_API UDissolveComponent : public UActorComponent
{
	GENERATED_BODY()

	// start declare events
public:
	DECLARE_EVENT(UDissolveComponent, FOnDissolveFinished)
	FOnDissolveFinished& OnDissolveFinished() { return DissolveFinishedEvent; }

private:
	FOnDissolveFinished DissolveFinishedEvent;
	// end declare events

private:
	UPROPERTY(EditInstanceOnly)
		UMaterialInstance* MI_Dissolve;

	UPROPERTY(EditInstanceOnly)
		FName DissolveValueName;

	UPROPERTY(EditInstanceOnly)
		FName DissolveColorName;

	UPROPERTY(EditInstanceOnly)
		float DissolveInterpSpeed;

	UPROPERTY(EditInstanceOnly)
		FLinearColor DissolveColor;

	UPROPERTY(EditInstanceOnly)
		TArray<FLinearColor> DissolvedComponents;
};
