#include "Player/ARLPlayerController.h"
#include "Blueprint/UserWidget.h"

void AARLPlayerController::TogglePauseMenu()
{
	if (!IsValid(PauseMenuClass))
	{
		return;
	}

	// Close if already open.
	if (IsValid(PauseMenuInstance) && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		// Open if it was closed.
		PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);

		// TODO: Come up with some zorder constants so we're not just guessing.
		// See UDeveloperSettings
		// https://docs.unrealengine.com/4.26/en-US/API/Runtime/DeveloperSettings/Engine/UDeveloperSettings/
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}

}

void AARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AARLPlayerController::TogglePauseMenu);
}

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
