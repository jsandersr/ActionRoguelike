#include "Player/ARLPlayerController.h"

void AARLPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChangedSignal.Broadcast(InPawn);
}

void AARLPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void AARLPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateChangedSignal.Broadcast(PlayerState);
}
