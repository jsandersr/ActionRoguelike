// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARLExplosiveBarrel::AARLExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->SetWorldLocation(GetActorLocation());

	RadialForceComp->SetAutoActivate(false);

	// If true, it ignores the mass of other objects.
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->Radius = ExplosionRadius;
	RadialForceComp->ImpulseStrength = ExplosionStrength;
}

// Called when the game starts or when spawned
void AARLExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void AARLExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &AARLExplosiveBarrel::OnHit);
}

void AARLExplosiveBarrel::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

	// %s is a placeholder for a string.
	// %f is a placeholder for a float.
	// logs: OtherActor: BP_PlayerCharacter_C_0, at game time: 0.000000
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"),
		*GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString LogString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());

	DrawDebugString(GetWorld(), Hit.ImpactPoint, LogString, nullptr, FColor::Green, 2.0f, true);
}

// Called every frame
void AARLExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
