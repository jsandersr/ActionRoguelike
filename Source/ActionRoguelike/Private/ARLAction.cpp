// Fill out your copyright notice in the Description page of Project Settings.

#include "ARLAction.h"

#include "../ActionRoguelike.h"
#include "ARLActionComponent.h"
#include "Net/UnrealNetwork.h"

void UARLAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	LogOnScreen(this, FString::Printf(TEXT("Started: %s"),
		*ActionName.ToString()), FColor::Green);

	UARLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantedTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStartedSeconds = GetWorld()->TimeSeconds;
	}


	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void UARLAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));

	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"),
		*ActionName.ToString()), FColor::Green);

	UARLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantedTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool UARLAction::CanStart_Implementation(AActor* Instigator)
{
	return CanStartInternal();
}

UWorld* UARLAction::GetWorld() const
{
	UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());
	return ActorComp ? ActorComp->GetWorld() : nullptr;
}

UARLActionComponent* UARLAction::GetOwningComponent() const
{
	return Cast<UARLActionComponent>(GetOuter());
}

void UARLAction::OnRep_RepDataChanged()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool UARLAction::CanStartInternal() const
{
	UARLActionComponent* Comp = GetOwningComponent();
	return !Comp->ActiveGameplayTags.HasAny(BlockedTags)/* && !bIsRunning*/;
}

void UARLAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARLAction, RepData);
	DOREPLIFETIME(UARLAction, TimeStartedSeconds);
}
