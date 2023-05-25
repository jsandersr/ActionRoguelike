// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	Health = MaxHealth;
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

