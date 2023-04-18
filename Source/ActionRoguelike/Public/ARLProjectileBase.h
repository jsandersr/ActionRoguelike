// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLProjectileBase.generated.h"

class USphereComponent;
class UParticleSystem;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API AARLProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AARLProjectileBase();

protected:
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void PostInitializeComponents() override;

	// This can be called when the projectile has reached its destination and before it is destroyed.
	// Use this to display explosion effects, fizzles, etc.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ExecuteEffect();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX = nullptr;

	// Our collision sphere.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp = nullptr;

	// Responsible for our movement.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp = nullptr;

	// VFX component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp = nullptr;
};
