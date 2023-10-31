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

	UPROPERTY(EditAnywhere)
	float TargetPitch = 0.0f;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh = nullptr;

	// KeyNote: // RepNotified is ReplicatedUsing
	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly)
	bool bLidOpened = false;

	UFUNCTION()
	void OnRep_LidOpened();

private:
	// BlueprintNativeEvent expects <function name>_Implementation.
	void Interact_Implementation(APawn* InstigatorPawn);

};
