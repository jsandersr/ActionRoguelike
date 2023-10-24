// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ARLGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARLGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAllAI();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve = nullptr;

	UFUNCTION()
	void OnSpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

protected:
	FTimerHandle TimerHandle_SpawnBots;

};
