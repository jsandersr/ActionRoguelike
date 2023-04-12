// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARLGameplayInterface.h"

#include "ARLItemChest.generated.h"

class UStaticMeshComponent;
UCLASS()
class ACTIONROGUELIKE_API AARLItemChest : public AActor, public IARLGameplayInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARLItemChest();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh = nullptr;

private:
	// This weirdness is because we made it a BlueprintNativeEvent in the interface.
	void Interact_Implementation(APawn* InstigatorPawn);

};
