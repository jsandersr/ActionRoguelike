// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"

#include "ARLGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UARLSaveGame;
class UDataTable;
class UARLMonsterData;

// Every data table MUST define a struct for its row structure.
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

// Each of the fields in here will be a column in the data table.
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UARLMonsterData* MonsterData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward = 20.0f;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARLGameModeBase();

	void InitGame(const FString& MapName, const FString& Options,
		FString& ErrorMessage) override;

	void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAllAI();

	void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

protected:
	UFUNCTION()
	void OnSpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	int32 CreditsPerKill = 0;

	FString SlotName = "SaveGame01";

	UPROPERTY()
	UARLSaveGame* CurrentSaveGame = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable = nullptr;

protected:
	FTimerHandle TimerHandle_SpawnBots;

};
