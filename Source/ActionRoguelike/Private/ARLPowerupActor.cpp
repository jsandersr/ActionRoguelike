// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPowerupActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AARLPowerupActor::AARLPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;

	SetReplicates(true);
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

FText AARLPowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void AARLPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in the derived classes...
}
