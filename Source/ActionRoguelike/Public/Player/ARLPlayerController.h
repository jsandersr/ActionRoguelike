#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "ARLPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChangedSignal, APawn*, NewPawn);

// We would need to do this for gamestate as well if we're listening to changed in UI. 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignal, APlayerState*, NewPlayerState);

UCLASS()
class ACTIONROGUELIKE_API AARLPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChangedSignal OnPawnChangedSignal;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangedSignal OnPlayerStateChangedSignal;

	void SetPawn(APawn* InPawn) override;

	// By default this function is not exposed to blueprint, so if we want to call it there
	// we need to expose it ourselves.
	void BeginPlayingState() override;

	// For initializing things like UI  since BeginPlay is likely too early for scenarios like
	// multiplayer clients, were not all data such as PlayerState has been received yet.
	// Follow this pattern for GameState as well.
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	// They made the OnRep but didn't bother to make an event.
	void OnRep_PlayerState() override;
};
