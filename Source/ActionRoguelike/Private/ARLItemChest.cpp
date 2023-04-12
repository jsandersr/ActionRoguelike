// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLItemChest.h"
#include "Components/StaticMeshComponent.h"


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
}

// Called when the game starts or when spawned
void AARLItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARLItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARLItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(110, 0, 0));
}
