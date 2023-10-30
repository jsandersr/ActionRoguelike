#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARLPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCreditsChangedSignal, AARLPlayerState*, PlayerState,
	int32, NewCredits, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API AARLPlayerState  : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const { return Credits; }

	UPROPERTY(BlueprintAssignable, Category = "Events");
	FCreditsChangedSignal CreditsChangedSignal;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits = 0;
};

