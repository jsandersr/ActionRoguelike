// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameModeBase.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ARLAICharacter.h"
#include "ARLAttributeComponent.h"
#include "Player/ARLPlayerState.h"

static TAutoConsoleVariable<bool> CVarSpawnBotsEnabled(TEXT("arl.SpawnBotsEnabled"), false,
	TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


AARLGameModeBase::AARLGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	PlayerStateClass = AARLPlayerState::StaticClass();
}


void AARLGameModeBase::StartPlay()
{
	Super::StartPlay();


	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARLGameModeBase::OnSpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void AARLGameModeBase::KillAllAI()
{
	for (AARLAICharacter* Bot : TActorRange<AARLAICharacter>(GetWorld()))
	{
		UARLAttributeComponent* AIAttributeComponent = UARLAttributeComponent::GetAttributeComponentFromActor(Bot);
		if (ensure(AIAttributeComponent) && AIAttributeComponent->IsAlive())
		{
			AIAttributeComponent->Kill(this); // TODO: Pass In Player
		}
	}
}

void AARLGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AARLAICharacter* Player = Cast<AARLAICharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, 2.0f, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		AARLPlayerState* PlayerState = KillerPawn->GetPlayerState<AARLPlayerState>();
		if (PlayerState)
		{
			PlayerState->AddCredits(CreditsPerKill);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void AARLGameModeBase::OnSpawnBotTimerElapsed()
{
	if (!CVarSpawnBotsEnabled.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarCpawnBots'."));

		return;
	}

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

void AARLGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}
