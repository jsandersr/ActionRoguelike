// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAICharacter.h"

// Sets default values
AARLAICharacter::AARLAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AARLAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARLAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AARLAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
