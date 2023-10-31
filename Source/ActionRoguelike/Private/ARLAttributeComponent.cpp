// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAttributeComponent.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "ARLGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("arl.DamageMultiplier"),
	1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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
	// Key Note: Attributes should have replication enabled in the constructor. When replication
	// is enabeld in the constructor, it should be enabled like this instead of SetIsReplication(true);
	SetIsReplicatedByDefault(true);
}

bool UARLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	if (DeltaHealth < 0.0f && !GetOwner()->CanBeDamaged())
	{
		return false;
	}

	if (DeltaHealth < 0.0)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		DeltaHealth *= DamageMultiplier;
	}

	float OldHealth = Health;
	Health = FMath::Clamp(Health + DeltaHealth, 0.0f, HealthMax);

	float ActualDeltaHealth = Health - OldHealth;
	bool bDidHealthChange = ActualDeltaHealth != 0;
	if (bDidHealthChange)
	{
		//HealthChangedSignal.Broadcast(InstigatorActor, this, Health, ActualDeltaHealth);

		// TODO: This is called for client and server. Figure out how to only call it on the server.
		MulticastHealthChanged(InstigatorActor, Health, ActualDeltaHealth);
	}

	if (ActualDeltaHealth < 0.0f && Health == 0.0f)
	{
		AARLGameModeBase* GM = GetWorld()->GetAuthGameMode<AARLGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return bDidHealthChange;
}

bool UARLAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

void UARLAttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();

	HealthChangedSignal.AddDynamic(this, &UARLAttributeComponent::OnHealthChanged);
}

void UARLAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float DeltaHealth)
{
	HealthChangedSignal.Broadcast(InstigatorActor, this, NewHealth, DeltaHealth);
}

void UARLAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARLAttributeComponent, Health);
	DOREPLIFETIME(UARLAttributeComponent, HealthMax);
	//DOREPLIFETIME_CONDITION(UARLAttributeComponent, HealthMax, COND_InitialOnly)
}

void UARLAttributeComponent::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
	float NewHealth, float DeltaHealth)
{
	// Example of how to connect to a delegate with a member function.
	// If this event is used in blueprint, then this function will NOT be called unless the
	// it is called from the blueprint.
}

