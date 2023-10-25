// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARLAction.h"
#include "ARLAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;
class AARLCharacter;

UCLASS()
class ACTIONROGUELIKE_API UARLAction_ProjectileAttack : public UARLAction
{
	GENERATED_BODY()

public:
	UARLAction_ProjectileAttack();
	virtual void StartAction_Implementation(AActor* InstigatorActor) override;

protected:
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AnimDelaySeconds = 0.2f;

};
