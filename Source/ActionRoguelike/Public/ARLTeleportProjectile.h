// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLProjectileBase.h"
#include "ARLTeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLTeleportProjectile : public AARLProjectileBase
{
	GENERATED_BODY()

public:
	AARLTeleportProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void ExecuteEffect_Implementation() override;

protected:
	void TeleportInstigator();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelaySeconds = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelaySeconds = 0.0f;

	// Called when the projectile has been fired.
	UFUNCTION(BlueprintImplementableEvent, Category = "TeleportEvents")
	void OnTeleportInitiated();

	// Called when the player has been teleported.
	UFUNCTION(BlueprintImplementableEvent, Category = "TeleportEvents")
	void OnTeleportResolved();

	// Called when the projectile has been detonated and before the player has been teleported.
	UFUNCTION(BlueprintImplementableEvent, Category = "TeleportEvents")
	void OnProjectileDetonated();

private:
	FTimerHandle TimerHandle_DelayedDetonate;
};
