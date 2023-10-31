// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLExplosiveBarrel.generated.h"

class URadialForceComponent;
class UStaticMeshComponent;
UCLASS()
class ACTIONROGUELIKE_API AARLExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AARLExplosiveBarrel();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called just before BeginPlay.
	virtual void PostInitializeComponents() override;

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URadialForceComponent* RadialForceComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	float ExplosionRadius = 200.0f;

	UPROPERTY(VisibleAnywhere)
	float ExplosionStrength = 2000.0f;
};
