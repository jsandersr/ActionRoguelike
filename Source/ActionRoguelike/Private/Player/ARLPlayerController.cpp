#include "Player/ARLPlayerController.h"

void AARLPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChangedSignal.Broadcast(InPawn);
}
