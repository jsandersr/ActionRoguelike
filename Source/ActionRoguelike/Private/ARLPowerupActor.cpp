// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPowerupActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AARLPowerupActor::AARLPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;
}

void AARLPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void AARLPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AARLPowerupActor::ShowPowerup, RespawnTime);
}

void AARLPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	RootComponent->SetVisibility(bNewIsActive, true);
}

void AARLPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in the derived classes...
}
