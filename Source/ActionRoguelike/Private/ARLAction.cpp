// Fill out your copyright notice in the Description page of Project Settings.

#include "ARLAction.h"
#include "ARLActionComponent.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));


	UARLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantedTags);

	bIsRunning = true;
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
	ensureAlways(bIsRunning);

	UARLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantedTags);

	bIsRunning = false;
}

bool UARLAction::CanStart_Implementation(AActor* Instigator)
{
	return CanStartInternal();
}

UWorld* UARLAction::GetWorld() const
{
	UActorComponent* ActorComp = Cast<UActorComponent>( GetOuter());
	return ActorComp ? ActorComp->GetWorld() : nullptr;
}

UARLActionComponent* UARLAction::GetOwningComponent() const
{
	return Cast<UARLActionComponent>(GetOuter());
}

bool UARLAction::CanStartInternal() const
{
	UARLActionComponent* Comp = GetOwningComponent();
	return !Comp->ActiveGameplayTags.HasAny(BlockedTags)/* && !bIsRunning*/;
}
