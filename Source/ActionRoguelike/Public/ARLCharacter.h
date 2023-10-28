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
class UARLActionComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARLCharacter();

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

protected:

	// Any sort of pregame hookups should be done here, not in the constructor.
	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

	// Moves character forward based on Value (which moves backward if negative).
	void MoveForward(float Value);

	// Strafes character right based on Value (which strafes left if negative).
	void MoveRight(float Value);

	// Uses character's primary attack.
	void UsePrimaryAbility();

	void UseTeleportAbility();
	
	void UseBlackHoleAbility();

	void UsePrimaryInteract();

	void SprintStart();

	void SprintStop();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
		float NewHealth, float DeltaHealth);

protected:
	// This is the animation that will play when we attack.
	UPROPERTY(EditAnywhere, Category = "Ability")
	UAnimMontage* AttackAnim = nullptr;

	// Camera attaches to this.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp = nullptr;

	// Camera that the "player" sees through.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UARLInteractionComponent* InteractionComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARLAttributeComponent* AttributeComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARLActionComponent* ActionComp = nullptr;

private:
	// The timer that will be used to trigger the primary attack.
	FTimerHandle TimerHandle_PrimaryAttack;

	// The timer that will be trigger the teleport ability.
	FTimerHandle TimerHandle_TeleportAbility;

	// The timer that will be used to trigger the black hole attack.
	FTimerHandle TimerHandle_BlackHoleAttack;
};
