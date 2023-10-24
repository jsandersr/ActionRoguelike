// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/ARLGameplayFunctionLibrary.h"
#include "ARLAttributeComponent.h"

bool UARLGameplayFunctionLibrary::ApplyDamage(AActor* InstigatorActor, AActor* TargetActor, float DamageAmount)
{
	UARLAttributeComponent* AttributeComponent =
		UARLAttributeComponent::GetAttributeComponentFromActor(TargetActor);
	if (AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(InstigatorActor, -DamageAmount);
	}

	return false;
}

bool UARLGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* InstigatorActor,
	AActor* TargetActor, float DamageAmount, const FHitResult& HitResult, float DirectionalForce)
{
	if (ApplyDamage(InstigatorActor, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComponent->AddImpulseAtLocation(-HitResult.ImpactNormal * DirectionalForce,
			HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
