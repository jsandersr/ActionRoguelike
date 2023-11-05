#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARLAnimInstance.generated.h"

class UARLActionComponent;

UCLASS()
class ACTIONROGUELIKE_API UARLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UARLActionComponent* ActionComp = nullptr;

	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;
};
