#include "ARLActionEffect.h"
#include "ARLActionComponent.h"
#include "GameFramework/GameStateBase.h"

UARLActionEffect::UARLActionEffect()
{
	static int32 IdGenerator = 0;
	
	Id = IdGenerator++;
	bAutoStart = true;
}

float UARLActionEffect::GetTimeRemainingSeconds() const
{
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (IsValid(GameState))
	{
		float EndTimeSeconds = TimeStartedSeconds + DurationSeconds;
		return EndTimeSeconds - GameState->GetServerWorldTimeSeconds();
	}

	return DurationSeconds;
}

void UARLActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (DurationSeconds > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate,
			DurationSeconds, false);
	}

	if (PeriodSeconds > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, PeriodSeconds, true);
	}
}

void UARLActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// Run this so we don't miss out on a last tick.
	if (GetWorld()->GetTimerManager().
		GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	UARLActionComponent* Comp = GetOwningComponent();
	if (IsValid(Comp))
	{
		Comp->RemoveAction(this);
	}
}

void UARLActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
