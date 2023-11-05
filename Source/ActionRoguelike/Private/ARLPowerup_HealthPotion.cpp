// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPowerup_HealthPotion.h"
#include "ARLAttributeComponent.h"
#include "Player/ARLPlayerState.h"

#include "Components/StaticMeshComponent.h"

// key note: you can use this define to define the localization namespace for the file.
#define LOCTEXT_NAMESPACE "InteractableActors"

AARLPowerup_HealthPotion::AARLPowerup_HealthPotion()
{

}

void AARLPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UARLAttributeComponent* AttributeComp =
		Cast<UARLAttributeComponent>(InstigatorPawn->GetComponentByClass(UARLAttributeComponent::StaticClass()));

	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AARLPlayerState* PlayerState = InstigatorPawn->GetPlayerState<AARLPlayerState>())
		{
			// TODO: This will remove credits even if applying the health change fails.
			if (PlayerState->RemoveCredits(CreditCost) &&
				AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}


FText AARLPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UARLAttributeComponent* AttributeComp =
		Cast<UARLAttributeComponent>(InstigatorPawn->GetComponentByClass(UARLAttributeComponent::StaticClass()));

	if (ensure(AttributeComp) && AttributeComp->IsFullHealth())
	{
		// key note: localization example
		                // category (up to you)  key (must be unique)             the text you want
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	// key note if you don't #define the loc namespace, then you would have to specify it in the
	// call.
	// key note: localization example
					// w/e namespace/category   key (must be unique)             the text you want
	// return NSLOCTEXT("InteractableActors", "HealthPotion_FullHealthWarning", "Already at full health.");

	// key note: localization requires this method of adding params. You can't just printf or w/e/.
	return FText::Format(LOCTEXT( "HealthPotion_InteractMessage",
		"Cost {0} Credits. Restores health to maximum."), CreditCost);
}

#undef LOCTEXT_NAMESPACE
