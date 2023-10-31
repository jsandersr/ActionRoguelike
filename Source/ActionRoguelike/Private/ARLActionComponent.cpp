// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionComponent.h"
#include "ARLAction.h"

#include <algorithm>

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

	for (TSubclassOf<UARLAction> ActionClass: DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UARLActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple(true);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void UARLActionComponent::AddAction(AActor* InstigatorActor, TSubclassOf<UARLAction> ActionToAdd)
{
	if (!ensure(ActionToAdd))
	{
		return;
	}

	UARLAction* NewAction = NewObject<UARLAction>(this, ActionToAdd);
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

	if (Action && (*Action)->IsRunning())
	{
		(*Action)->StopAction(InstigatorActor);
		return true;
	}

	return false;
}

void UARLActionComponent::ServerStartAction_Implementation(AActor* InstigatorActor, FName ActionName)
{
	StartActionByName(InstigatorActor, ActionName);
}
