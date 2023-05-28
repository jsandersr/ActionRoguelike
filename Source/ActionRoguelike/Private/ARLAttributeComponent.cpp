// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"

static const float SHealthMax = 100.f;

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
	: Health(SHealthMax)
	, HealthMax(SHealthMax)
{
}

bool UARLAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	float CurrentHealth = Health;
	Health = FMath::Clamp(Health + DeltaHealth, 0.0f, HealthMax);

	bool bDidHealthChange = CurrentHealth != Health;
	if (bDidHealthChange)
	{
		HealthChangedSignal.Broadcast(GetOwner(), this, Health, DeltaHealth);
	}

	return bDidHealthChange;
}

void UARLAttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();

	HealthChangedSignal.AddDynamic(this, &UARLAttributeComponent::HandleOnHealthChanged);
}

void UARLAttributeComponent::HandleOnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
	float NewHealth, float DeltaHealth)
{
	// Example of how to connect to a delegate with a member function.
}
