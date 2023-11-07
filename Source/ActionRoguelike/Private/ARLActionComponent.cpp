// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionComponent.h"

#include "../ActionRoguelike.h"
#include "ARLAction.h"
#include "Net/UnrealNetwork.h"

#include <algorithm>
#include "Engine/ActorChannel.h"


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);

// Sets default values for this component's properties
UARLActionComponent::UARLActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UARLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UARLAction> ActionClass: DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UARLActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple(true);
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Very, very expensive.
	// for (UARLAction* Action : Actions)
	// {
	// 	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	// 	FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),
	// 		*GetNameSafe(GetOwner()),
	// 		*GetNameSafe(Action));
	// 		//*LexToString(Action->IsRunning()),
	// 		//*GetNameSafe(Action->GetOuter()));

	// 	LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	// }
}

void UARLActionComponent::AddAction(AActor* InstigatorActor, TSubclassOf<UARLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// Skip for clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"),
			*GetNameSafe(ActionClass));
		return;
	}

	UARLAction* NewAction = NewObject<UARLAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		// TODO: Ideally we have an ensure around CanStart. But currently
		// if you add BlockTags to the ActionEffect, it triggers. Will need
		// a broader fix.
		if (NewAction->bAutoStart && NewAction->CanStart(InstigatorActor))
		{
			NewAction->StartAction(InstigatorActor);
		}
	}
}

void UARLActionComponent::RemoveAction(UARLAction* ActionToRemove)
{
	if (!ensure(IsValid(ActionToRemove) && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

// TODO: Currently, the animation played is inconsistent between client/server.
// If action happened on client, the server sees it in real time no matter how fast.
// If it happens on server, the client only sees it after IsRunning has returned to false.
// Investigate.
bool UARLActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	//Key note: You can use STL Algos but iterators are not well supported(nonexistent).
	// You can use GetData() instead though.
	//auto Found = std::find_if(Actions.GetData(), Actions.GetData() + Actions.Num(),
	//	[&ActionName](UARLAction* CurrentAction)
	//{
	//	return IsValid(CurrentAction) && CurrentAction->ActionName == ActionName;
	//});
	//if (Found != Actions.GetData() + Actions.Num())
	//{
	//	return false;
	//}

	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	UARLAction** Action = Actions.FindByPredicate(
		[&ActionName](UARLAction* CurrentAction)
	{
		return CurrentAction && CurrentAction->ActionName == ActionName;
	});

	if (Action && IsValid(*Action) && (*Action)->CanStart(InstigatorActor))
	{
		if (!GetOwner()->HasAuthority())
		{
			ServerStartAction(InstigatorActor, ActionName);
		}

		TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(*Action));

		(*Action)->StartAction(InstigatorActor);
		return true;
	}

	FString FailedMessage = FString::Printf(TEXT("Failed to run: %s)"), *ActionName.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMessage);
	return false;
}

bool UARLActionComponent::StopActionByName(AActor* InstigatorActor, FName ActionName)
{
	UARLAction** Action = Actions.FindByPredicate(
		[&ActionName](UARLAction* CurrentAction)
	{
		return CurrentAction && CurrentAction->ActionName == ActionName;
	});

	if (Action && IsValid(*Action) && (*Action)->IsRunning())
	{
		if (!GetOwner()->HasAuthority())
		{
			ServerStopAction(InstigatorActor, ActionName);
		}

		(*Action)->StopAction(InstigatorActor);
		return true;
	}

	return false;
}

bool UARLActionComponent::ReplicateSubobjects(class UActorChannel* Channel,
	class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UARLAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UARLActionComponent::ServerStartAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StartActionByName(InstigatorActor, ActionName);
}

void UARLActionComponent::ServerStopAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StopActionByName(InstigatorActor, ActionName);
}

void UARLActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UARLActionComponent, Actions)
}
