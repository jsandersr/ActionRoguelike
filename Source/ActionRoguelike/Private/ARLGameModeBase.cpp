// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLGameModeBase.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/ARLAICharacter.h"
#include "ARLAttributeComponent.h"
#include "Player/ARLPlayerState.h"
#include <Kismet/GameplayStatics.h>
#include "ARLSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "ARLGameplayInterface.h"
#include "ARLMonsterData.h"
#include "../ActionRoguelike.h"
#include "ARLActionComponent.h"
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>
#include <Engine/AssetManager.h>

static TAutoConsoleVariable<bool> CVarSpawnBotsEnabled(TEXT("arl.SpawnBotsEnabled"), true,
	TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


AARLGameModeBase::AARLGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	PlayerStateClass = AARLPlayerState::StaticClass();
}


void AARLGameModeBase::InitGame(const FString& MapName, const FString& Options,
	FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	LoadSaveGame();
}

void AARLGameModeBase::StartPlay()
{
	Super::StartPlay();


	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARLGameModeBase::OnSpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void AARLGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	auto* PlayerState = NewPlayer->GetPlayerState<AARLPlayerState>();
	if (IsValid(PlayerState))
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}

	// call this after so we load as early as possible.
	// This results in ::BeginPlayingState, which could make our init code run too late.
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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

void AARLGameModeBase::WriteSaveGame()
{
	for (TObjectPtr<APlayerState> PlayerStatePtr : GameState->PlayerArray)
	{
		auto* PlayerState = Cast<AARLPlayerState>(PlayerStatePtr.Get());
		if (IsValid(PlayerState))
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (IsValid(Actor) && Actor->Implements<UARLGameplayInterface>())
		{
			FActorSaveData ActorData;
			ActorData.ActorName = Actor->GetName();
			ActorData.Transform = Actor->GetActorTransform();

			// Pass the array to fill with data from actor.
			FMemoryWriter MemWriter(ActorData.ByteData);

			FObjectAndNameAsStringProxyArchive Archive(MemWriter, true);

			// Tells the FMemoryWriter that we're only dealing with save game data.
			// This will cause it to only consider UPROPERTY(SaveGame) fields.
			Archive.ArIsSaveGame = true;

			// Converts Actor's SaveGame UPROPERTYs into binary array.
			Actor->Serialize(Archive);
			
			CurrentSaveGame->SavedActors.Add(ActorData);
		}
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AARLGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UARLSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (!CurrentSaveGame)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
	}
	else
	{
		CurrentSaveGame = Cast<UARLSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UARLSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame Data."));
	}

	// Look through all of the actors in the game (could be tens of thousands)
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		// When we find one that's relevant, find saved data match and init the actor.
		if (IsValid(Actor) && Actor->Implements<UARLGameplayInterface>())
		{
		// TODO figure out a TMap solution instead.
			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);


					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Archive(MemReader, true);

					Archive.ArIsSaveGame;
					// Convert binary array back into actor's variables.
					Actor->Serialize(Archive);

					// Key Note: when we're using an interface like this, we use "I" but
					// when casting, we use "U". eg. Actor->Implements<UARLGameplayInterface>()
					IARLGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
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

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,
		SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,
			&AARLGameModeBase::OnSpawnBotQueryComplete);
	}

}

void AARLGameModeBase::OnSpawnBotQueryComplete(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
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
	
	TArray<FMonsterInfoRow*> Rows;
	MonsterTable->GetAllRows("SpawningMinions", Rows);

	int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
	FMonsterInfoRow* SelectedRow = Rows[RandomIndex];
	
	if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{

		LogOnScreen(this, FString::Printf(TEXT("Loading monster from id type=%s name=%s"),
			*SelectedRow->MonsterId.PrimaryAssetType.ToString(),
			*SelectedRow->MonsterId.PrimaryAssetName.ToString()), FColor::Cyan);

		TArray<FName> Bundles;
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, 
			&AARLGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId,
			Locations[0]);

		AssetManager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
	}
}

void AARLGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void AARLGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	UARLMonsterData* MonsterData = nullptr;
	if (UAssetManager* AssetManager = UAssetManager::GetIfValid())
	{
		MonsterData = Cast<UARLMonsterData>(AssetManager->GetPrimaryAssetObject(LoadedId));
		if (!MonsterData)
		{
			return;
		}

		LogOnScreen(this, "MonsterData loaded...", FColor::Green);
	}

	AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
	if (!NewBot)
	{
		return;
	}

	LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"),
		*GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

	UARLActionComponent* ActionComp = Cast<UARLActionComponent>(
		NewBot->GetComponentByClass(UARLActionComponent::StaticClass()));

	if (IsValid(ActionComp))
	{
		for (TSubclassOf<UARLAction> ActionClass : MonsterData->Actions)
		{
			ActionComp->AddAction(NewBot, ActionClass);
		}
	}
}
