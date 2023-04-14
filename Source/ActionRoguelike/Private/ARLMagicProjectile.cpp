// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ARLAttributeComponent.h"

// Sets default values
AARLMagicProjectile::AARLMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Different ways to access collision channels.
	//SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Only interested in overlapping with pawn example:
	// SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");

	// We can use a collision profile specific to projectiles.
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AARLMagicProjectile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void AARLMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
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

// Called every frame
void AARLMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
