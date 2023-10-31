// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLInteractionComponent.generated.h"

class UARLWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UARLInteractionComponent();

	void PrimaryInteract();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void FindBestInteractable();

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* CurrentFocusActor);

protected:

	UPROPERTY()
	AActor* FocusedActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARLWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UARLWorldUserWidget* DefaultWidgetInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;

};
