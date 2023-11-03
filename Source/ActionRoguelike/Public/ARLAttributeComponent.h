// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARLAttributeComponent.generated.h"

// TODO:
// If you had multiple attributes (like rage, mana, etc), you can update any of them with a single
// AttributeChanged rather than, OnHealthChanged, OnManaChanged, etc
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAttributeChangedSignal, AActor*, InstigatorActor, UARLAttributeComponent*, OwningComp, float, NewValue, float, DeltaValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealthChangedSignal, AActor*, InstigatorActor, UARLAttributeComponent*, OwningComp, float, NewHealth, float, DeltaHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UARLAttributeComponent* GetAttributeComponentFromActor(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (Display = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);


	// Sets default values for this component's properties
	UARLAttributeComponent();

	// Called to apply a change to the health of the owning actor.
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth);

	// Delegate event that notifies when health changes.
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FHealthChangedSignal HealthChangedSignal;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const { return Health > 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax() const { return HealthMax; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const { return Health >= HealthMax; }

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

public:
	// Any sort of pregame hookups should be done here, not in the constructor.
	void InitializeComponent() override;
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// EditAnywhere - edit in BP editor and per-instance level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only.
	// VisibleDefaultsOnly - 'read-only' access for a variable, only in BP editor.
	// BlueprintReadOnly - read-only in the Blueprint scrupting( does not affect 'details' panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// Category = "" display only for detail panels and blueprint context menu.

	// Health of the owning actor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	// Max health of the owning actor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float HealthMax;

	// Key note: We want to mark MulticastHealthChanged as unreliable because this RPC is
	// basically just for cosmetic changes anyway. The actual properties are
	// already successfully being replicated elsewhere. Also Reliable Multicasts ignore relevancy.

	// key note: use multicast for transient events that happen in the moment and don't result
	// in game/player state changes (so for cosmetic things). Use rep_notifys for state changes.

	// TODO: ARLCharacter uses health value to disable input. Until we fix that this has to be Reliable.
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float DeltaHealth);

private:
	// Handles health change events. (NYI, just an example).
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
		float NewHealth, float DeltaHealth);
};
