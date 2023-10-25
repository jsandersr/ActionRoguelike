// Fill out your copyright notice in the Description page of Project Settings.

#include "ARLAction.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
}

UWorld* UARLAction::GetWorld() const
{
	UActorComponent* ActorComp = Cast<UActorComponent>( GetOuter());
	return ActorComp ? ActorComp->GetWorld() : nullptr;
}
