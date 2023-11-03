#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "ARLPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChangedSignal, APawn*, NewPawn);

UCLASS()
class ACTIONROGUELIKE_API AARLPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChangedSignal OnPawnChangedSignal;

	void SetPawn(APawn* InPawn) override;
};
