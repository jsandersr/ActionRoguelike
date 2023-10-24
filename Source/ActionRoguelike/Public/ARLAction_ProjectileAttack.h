// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAction.h"
#include "ARLAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API UARLAction_ProjectileAttack : public UARLAction
{
	GENERATED_BODY()

public:


protected:
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Ability")
	UAnimMontage* AttackAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ability")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Ability")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float AnimDelaySeconds = 0.2f;


};
