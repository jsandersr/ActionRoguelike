#include "Player/ARLPlayerState.h"
#include "ARLSaveGame.h"

void AARLPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;
	CreditsChangedSignal.Broadcast(this, Credits, Delta);
}

bool AARLPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}
	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;
	CreditsChangedSignal.Broadcast(this, Credits, -Delta);

	return true;
}

void AARLPlayerState::SavePlayerState_Implementation(UARLSaveGame* SaveObject)
{
	if (IsValid(SaveObject))
	{
		SaveObject->Credits = Credits;
	}
}

void AARLPlayerState::LoadPlayerState_Implementation(UARLSaveGame* SaveObject)
{
	if (IsValid(SaveObject))
	{
		Credits = SaveObject->Credits;
	}
}
