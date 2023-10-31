// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ARLAction.generated.h"

class UWorld;
class UARLActionComponent;

/**
 * 
 */
 // Allows us to make child classes from the editor.
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARLAction : public UObject
{
	GENERATED_BODY()

//Functions
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action") /* const*/
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const { return bIsRunning; }

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override { return true; }

// Fields
public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart = false;

// Functions
protected:
	UFUNCTION(BlueprintCallable, Category = "Action")
	UARLActionComponent* GetOwningComponent() const;

// Fields
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;


// Networking
protected:
	// Flag enforces only a single action at a time can ever run.
	// If multiple actions should run at the same time, we should remove this
	// and just utilize tags.
	UPROPERTY(ReplicatedUsing="OnRep_IsRunningChanged")
	bool bIsRunning = false;

	UFUNCTION()
	void OnRep_IsRunningChanged();

private:
	bool CanStartInternal() const;

};
