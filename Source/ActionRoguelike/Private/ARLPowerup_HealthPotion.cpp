// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLPowerup_HealthPotion.h"
#include "ARLAttributeComponent.h"
#include "Player/ARLPlayerState.h"

#include "Components/StaticMeshComponent.h"

AARLPowerup_HealthPotion::AARLPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

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
