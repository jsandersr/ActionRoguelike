// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "ARLProjectileBase.h"

#include "ARLMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UARLActionEffect;

UCLASS()
class ACTIONROGUELIKE_API AARLMagicProjectile : public AARLProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AARLMagicProjectile();

protected:
	virtual void PostInitializeComponents() override;

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Ability")
	float DamageAmount = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag ParryTag;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UARLActionEffect> BurningActionClass;
};
