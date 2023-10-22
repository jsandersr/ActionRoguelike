// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"

static const float SHealthMax = 100.f;

UARLAttributeComponent* UARLAttributeComponent::GetAttributeComponentFromActor(AActor* FromActor)
{
	return FromActor ? FromActor->FindComponentByClass<UARLAttributeComponent>() : nullptr;
}

bool UARLAttributeComponent::IsActorAlive(AActor* Actor)
{
	UARLAttributeComponent* AttributeComp = GetAttributeComponentFromActor(Actor);
	return AttributeComp && AttributeComp->IsAlive();
}

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
	: Health(SHealthMax)
	, HealthMax(SHealthMax)
{
}

bool UARLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + DeltaHealth, 0.0f, HealthMax);

	float ActualDeltaHealth = Health - OldHealth;
	bool bDidHealthChange = ActualDeltaHealth != 0;
	if (bDidHealthChange)
	{
		HealthChangedSignal.Broadcast(InstigatorActor, this, Health, ActualDeltaHealth);
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
	// If this event is used in blueprint, then this function will NOT be called unless the
	// it is called from the blueprint.
}
