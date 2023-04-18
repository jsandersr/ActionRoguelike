// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ARLAttributeComponent.h"

// Sets default values
AARLMagicProjectile::AARLMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Only interested in overlapping with pawn example:
	// SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MovementComp->InitialSpeed = 3000.0f;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AARLMagicProjectile::OnActorOverlap);
}

void AARLMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	UARLAttributeComponent* AttributeComp =
		Cast<UARLAttributeComponent>(OtherActor->GetComponentByClass(UARLAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		// TODO: don't hard code the health amount.
		AttributeComp->ApplyHealthChange(-20.0f);
		Destroy();
	}
}
