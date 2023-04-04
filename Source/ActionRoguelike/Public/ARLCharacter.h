// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Moves character forward based on Value (which moves backward if negative).
	void MoveForward(float Value);

	// Strafes character right based on Value (which strafes left if negative).
	void MoveRight(float Value);
};
