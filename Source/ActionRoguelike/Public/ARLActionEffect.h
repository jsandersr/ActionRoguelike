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

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemainingSeconds() const;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float DurationSeconds = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float PeriodSeconds = 0.f;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

private:
	int32 Id = 0;
};

