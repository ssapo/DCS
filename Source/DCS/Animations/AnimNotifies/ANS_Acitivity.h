#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Structs.h"
#include "ANS_Acitivity.generated.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;

UCLASS()
class DCS_API UANS_Acitivity : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UANS_Acitivity();

private:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FString GetNotifyName_Implementation() const override;

public:
	UPROPERTY(EditAnywhere)
		EActivity Activity;
};
