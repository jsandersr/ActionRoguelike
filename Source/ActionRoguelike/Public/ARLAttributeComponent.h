// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealthChangedSignal, AActor*, InstigatorActor, UARLAttributeComponent*, OwningComp, float, NewHealth, float, DeltaHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UARLAttributeComponent();

	// Called to apply a change to the health of the owning actor.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float DeltaHealth);

	// Delegate event that notifies when health changes.
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FHealthChangedSignal HealthChangedSignal;

public:
	// Any sort of pregame hookups should be done here, not in the constructor.
	virtual void InitializeComponent() override;

protected:
	// EditAnywhere - edit in BP editor and per-instance level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only.
	// VisibleDefaultsOnly - 'read-only' access for a variable, only in BP editor.
	// BlueprintReadOnly - read-only in the Blueprint scrupting( does not affect 'details' panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// Category = "" display only for detail panels and blueprint context menu.

	// Health of the owning actor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	// Max health of the owning actor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;

private:
	// Handles health change events. (NYI, just an example).
	UFUNCTION()
	void HandleOnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
		float NewHealth, float DeltaHealth);
};
