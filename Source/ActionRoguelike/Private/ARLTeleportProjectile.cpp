// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTeleportProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


AARLTeleportProjectile::AARLTeleportProjectile()
{
	TeleportDelaySeconds = 0.2f;
	DetonateDelaySeconds = 0.2f;

	MovementComp->InitialSpeed = 1500.0f;
}

void AARLTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this,
		&AARLTeleportProjectile::ExecuteEffect, DetonateDelaySeconds);
	AActor* ActorToTeleport = GetInstigator();

	FString LogString = FString::Printf(TEXT(
		"Teleport Initiated."
		" Player Pos: %s"
		" Projectile Pos %s"),
		*ActorToTeleport->GetActorLocation().ToString(), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Log, TEXT("%s"), *LogString);

	OnTeleportInitiated();
}

void AARLTeleportProjectile::ExecuteEffect_Implementation()
{
	// Clear timer if we were already called through somewhere else.
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	AActor* ActorToTeleport = GetInstigator();
	FString LogString = FString::Printf(TEXT(
		" Kicking off Teleport Timer."
		" Player Pos: %s"
		" Projectile Pos %s"),
		*ActorToTeleport->GetActorLocation().ToString(), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Log, TEXT("%s"), *LogString);

	OnProjectileDetonated();

	// We don't care about canceling this timer, it will be cleared when the actor is destroyed.
	FTimerHandle TimerHandle_DelayedTeleport;

	// Delay teleport to allow the impact VFX to play.
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this,
		&AARLTeleportProjectile::TeleportInstigator, TeleportDelaySeconds);
}

void AARLTeleportProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(),
			false, false);
	}

	FString LogString = FString::Printf(TEXT(
		" Resolving Teleport."
		" Player Pos: %s"
		" Projectile Pos %s"),
		*ActorToTeleport->GetActorLocation().ToString(), *GetActorLocation().ToString());

	UE_LOG(LogTemp, Log, TEXT("%s"), *LogString);

	OnTeleportResolved();
}
