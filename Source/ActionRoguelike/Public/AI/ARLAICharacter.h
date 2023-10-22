// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARLAICharacter.generated.h"

class UPawnSensingComponent;
class UARLAttributeComponent;
class HealthBarWidgetClass;
class UARLWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API AARLAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARLAICharacter();

	UFUNCTION(BlueprintCallable, Category = "API")
	void ShowHealthBar();

	void SetTargetActor(AActor* NewTarget);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
		float NewHealth, float DeltaHealth);

protected:

	UARLWorldUserWidget* ActiveHealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARLAttributeComponent* AttributeComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName = "HitTimestamp";

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

};
