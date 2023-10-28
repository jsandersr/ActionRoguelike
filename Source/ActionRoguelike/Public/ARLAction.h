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

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action") /* const*/
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const { return bIsRunning; }

	UWorld* GetWorld() const override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	UARLActionComponent* GetOwningComponent() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	// Flag enforces only a single action at a time can ever run.
	// If multiple actions should run at the same time, we should remove this
	// and just utilize tags.
	bool bIsRunning = false;

private:
	bool CanStartInternal() const;

};
