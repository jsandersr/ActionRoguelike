#pragma once

#include "CoreMinimal.h"
#include "ARLPowerupActor.h"
#include "ARLPowerup_Credits.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLPowerup_Credits : public AARLPowerupActor
{
	GENERATED_BODY()

protected:
	void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, Category = "Powerup")
	int32 CreditsAmount = 80;
};
