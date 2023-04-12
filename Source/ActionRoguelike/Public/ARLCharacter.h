// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UARLInteractionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API AARLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARLCharacter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Moves character forward based on Value (which moves backward if negative).
	void MoveForward(float Value);

	// Strafes character right based on Value (which strafes left if negative).
	void MoveRight(float Value);

	// Uses character's primary attack.
	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void PrimaryInteract();

protected:
	// This is a projectile that we will spawn.
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	// This is the animation that will play when we attack.
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim = nullptr;

	// This is the delay between attacks.
	UPROPERTY(EditAnywhere, Category = "Attack")
	float PrimaryAttackDelaySeconds = 0.2f;

	// Camera attaches to this.
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp = nullptr;

	// Camera that the "player" sees through.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	UARLInteractionComponent* InteractionComp = nullptr;

private:
	// This is the timer that will be used to spawn projectiles.
	FTimerHandle TimerHandle_PrimaryAttack;
};
