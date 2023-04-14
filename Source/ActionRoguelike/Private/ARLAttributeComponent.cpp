// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"

// Sets default values for this component's properties
UARLAttributeComponent::UARLAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = MaxHealth;
}

bool UARLAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	Health = FMath::Clamp(Health + DeltaHealth, 0.0f, MaxHealth);
	return Health <= 0.0f;
}

// Called when the game starts
void UARLAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UARLAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
