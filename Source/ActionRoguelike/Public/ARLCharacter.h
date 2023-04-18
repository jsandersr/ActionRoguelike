// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UARLInteractionComponent;
class UAnimMontage;
class UARLAttributeComponent;

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
	void UsePrimaryAbility();

	void OnPrimaryAbility_TimeElapsed();

	void UseTeleportAbility();
	
	void OnTeleportAbility_TimeElapsed();

	void UseBlackHoleAbility();

	void OnBlackHoleAbility_TimeElapsed();

	void UsePrimaryInteract();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

protected:
	// This is a projectile that we will spawn.
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<AActor> AttackAbilityProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<AActor> TeleportAbilityProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<AActor> BlackHoleAbilityProjectileClass;

	// This is the animation that will play when we attack.
	UPROPERTY(EditAnywhere, Category = "Ability")
	UAnimMontage* AttackAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float PrimaryAttackAbilityDelaySeconds = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float TeleportAbilityDelaySeconds = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float BlackHoleAbilityDelaySeconds = 0.2f;

	// Camera attaches to this.
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USpringArmComponent* SpringArmComp = nullptr;

	// Camera that the "player" sees through.
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* CameraComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UARLInteractionComponent* InteractionComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UARLAttributeComponent* AttributeComp = nullptr;

private:
	// The timer that will be used to trigger the primary attack.
	FTimerHandle TimerHandle_PrimaryAttack;

	// The timer that will be trigger the teleport ability.
	FTimerHandle TimerHandle_TeleportAbility;

	// The timer that will be used to trigger the black hole attack.
	FTimerHandle TimerHandle_BlackHoleAttack;
};
