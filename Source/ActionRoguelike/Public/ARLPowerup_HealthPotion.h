// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLPowerupActor.h"
#include "ARLPowerup_HealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARLPowerup_HealthPotion : public AARLPowerupActor
{
	GENERATED_BODY()
	
public:
	AARLPowerup_HealthPotion();

protected:

	UPROPERTY(EditAnywhere, Category = "Health Potion")
	int32 CreditCost = 0;

	public:
	virtual void Interact_Implementation(APawn* Interactor) override;
	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
