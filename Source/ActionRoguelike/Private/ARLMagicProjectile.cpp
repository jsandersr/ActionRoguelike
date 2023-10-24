// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ARLAttributeComponent.h"
#include "Utils/ARLGameplayFunctionLibrary.h"

static TAutoConsoleVariable<float> CVarDirectionalForce(TEXT("arl.MagicProjectile.DirectionalForce"),
	300000.f, TEXT("Amount of force from directional projectile."), ECVF_Cheat);

// Sets default values
AARLMagicProjectile::AARLMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Only interested in overlapping with pawn example:
	// SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MovementComp->InitialSpeed = 3000.0f;
}

void AARLMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AARLMagicProjectile::OnActorOverlap);
}

void AARLMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator())
		return;

	//UARLAttributeComponent* AttributeComp =
	//	Cast<UARLAttributeComponent>(OtherActor->GetComponentByClass(UARLAttributeComponent::StaticClass()));

	//if (AttributeComp)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Taking 20 damage"));
	//	// TODO: don't hard code the health amount.
	//	AttributeComp->ApplyHealthChange(GetInstigator(), - DamageAmount);
	//	ExecuteEffect();
	//	Destroy();
	//}

	float DirectionalForce = CVarDirectionalForce.GetValueOnGameThread();
	if (UARLGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult, DirectionalForce))
	{
		UE_LOG(LogTemp, VeryVerbose, TEXT("Taking %f damage"), DamageAmount);

		ExecuteEffect();
	}
}
