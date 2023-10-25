// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARLAction.generated.h"

class UWorld;

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

	UWorld* GetWorld() const override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

};
