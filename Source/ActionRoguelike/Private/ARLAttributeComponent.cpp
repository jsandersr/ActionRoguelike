// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	Health = MaxHealth;
	OnHealthChanged.AddDynamic(this, &UARLAttributeComponent::HandleOnHealthChanged);
}

bool UARLAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	float CurrentHealth = Health;
	Health = FMath::Clamp(Health + DeltaHealth, 0.0f, MaxHealth);

	bool bDidHealthChange = CurrentHealth != Health;
	if (bDidHealthChange)
	{
		OnHealthChanged.Broadcast(GetOwner(), this, Health, DeltaHealth);
	}

	return bDidHealthChange;
}
void UARLAttributeComponent::HandleOnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
	float NewHealth, float DeltaHealth)
{
	// Example of how to connect to a delegate with a member function.
}
