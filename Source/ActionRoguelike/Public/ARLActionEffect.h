#pragma once

#include "CoreMinimal.h"
#include "ARLAction.h"
#include "ARLActionEffect.generated.h"

UCLASS()
class ACTIONROGUELIKE_API UARLActionEffect : public UARLAction
{
	GENERATED_BODY()

public:

	UARLActionEffect();
	void StartAction_Implementation(AActor* Instigator) override;
	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period = 0.f;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

private:
	int32 Id = 0;
};

