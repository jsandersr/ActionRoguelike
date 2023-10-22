// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameModeBase.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ARLAICharacter.h"
#include "ARLAttributeComponent.h"

void AARLGameModeBase::StartPlay()
{
	Super::StartPlay();


	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARLGameModeBase::OnSpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void AARLGameModeBase::OnSpawnBotTimerElapsed()
{
	int32 NumOfAliveBots = 0;
	for (AARLAICharacter* Bot : TActorRange<AARLAICharacter>(GetWorld()))
	{
		UARLAttributeComponent* AIAttributeComponent = UARLAttributeComponent::GetAttributeComponentFromActor(Bot);
		if (ensure(AIAttributeComponent) && AIAttributeComponent->IsAlive())
		{
			++NumOfAliveBots;
		}
	}

	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}

	if (NumOfAliveBots >= MaxBotCount)
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this,
		EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARLGameModeBase::OnSpawnBotQueryComplete);
	}

}

void AARLGameModeBase::OnSpawnBotQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot query failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() <= 0)
	{
		return;
	}
	
	GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
}

AARLGameModeBase::AARLGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}
