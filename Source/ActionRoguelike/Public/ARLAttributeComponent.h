// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UARLAttributeComponent*, OwningComp, float, NewHealth, float, DeltaHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UARLAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float DeltaHealth);

public:
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
		FOnHealthChanged OnHealthChanged;


protected:
	// EditAnywhere - edit in BP editor and per-instance level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only.
	// VisibleDefaultsOnly - 'read-only' access for a variable, only in BP editor.
	// BlueprintReadOnly - read-only in the Blueprint scrupting( does not affect 'details' panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// Category = "" display only for detail panels and blueprint context menu.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth = 100.f;

private:
	UFUNCTION()
	void HandleOnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);
};
