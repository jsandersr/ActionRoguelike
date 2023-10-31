// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AARLItemChest::AARLItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;

	SetReplicates(true);
}

void AARLItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

// THIS IS RUN FROM THE SERVER
void AARLItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;

	// KeyNote:
	// UARLInteractionComponent::PrimaryInteract() calls UARLInteractionComponent::ServerInteract_Implementation
	// which dispatches the Interact_Implementation to here.

	// So this function is run on the SERVER. When implementing RepNotify in C++, the replication function
	// is NOT automatically called on the server. So we need to call it here.
	OnRep_LidOpened();
}

void AARLItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARLItemChest, bLidOpened);
}