// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ARLActionComponent.generated.h"

class UARLAction;
struct FGameplayTagContainer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	// Sets default values for this actor's properties
	UARLActionComponent();

	virtual void BeginPlay();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* InstigatorActor, TSubclassOf<UARLAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UARLAction* ActionToRemove);


	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* InstigatorActor, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* InstigatorActor, FName ActionName);

	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
		FReplicationFlags* RepFlags) override;

protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* InstigatorActor, FName ActionName);

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UARLAction>> DefaultActions;

	UPROPERTY(Replicated)
	TArray<UARLAction*> Actions;
};
