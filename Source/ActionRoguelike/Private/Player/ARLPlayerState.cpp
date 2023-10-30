#include "Player/ARLPlayerState.h"

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
