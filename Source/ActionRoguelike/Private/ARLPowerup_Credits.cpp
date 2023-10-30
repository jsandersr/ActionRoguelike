#include "ARLPowerup_Credits.h"
#include "Player/ARLPlayerState.h"

void AARLPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AARLPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AARLPlayerState>())
	{
		PlayerState->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}

