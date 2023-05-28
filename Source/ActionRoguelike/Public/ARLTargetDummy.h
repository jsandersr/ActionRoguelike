// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLTargetDummy.generated.h"

class UARLAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AARLTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AARLTargetDummy();

public:
	// Any sort of pregame hookups should be done here, not in the constructor.
	virtual void PostInitializeComponents() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARLAttributeComponent* AttributeComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp = nullptr;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
		float NewHealth, float DeltaHealth);
};
