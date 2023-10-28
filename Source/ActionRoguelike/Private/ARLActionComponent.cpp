// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLActionComponent.h"
#include "ARLAction.h"

// Sets default values for this component's properties
UARLActionComponent::UARLActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UARLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UARLAction> ActionClass: DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UARLActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple(true);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void UARLActionComponent::AddAction(TSubclassOf<UARLAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UARLAction* NewAction = NewObject<UARLAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UARLActionComponent::StartActionByName(AActor* InstigatorActor, FName ActionName)
{
	UARLAction** Action = Actions.FindByPredicate(
		[&ActionName](UARLAction* CurrentAction)
	{
		return CurrentAction && CurrentAction->ActionName == ActionName;
	});

	if (Action && (*Action)->CanStart(InstigatorActor))
	{
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
