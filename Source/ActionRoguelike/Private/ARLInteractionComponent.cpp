// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLInteractionComponent.h"
#include "ARLGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDrawDebugLinesEnabledInteraction(TEXT("arl.DrawDebugLinesEnabled_Interaction"),
	false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
UARLInteractionComponent::UARLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UARLInteractionComponent::PrimaryInteract()
{
	bool isDebugLinesEnabled = CVarDrawDebugLinesEnabledInteraction.GetValueOnGameThread();
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* OwningCharacter = GetOwner();

	FRotator EyeRotation;
	FVector EyeLocation;
	OwningCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	// You could detect hit with a simple ray cast. But this requires the player be pixel perfect with their
	// aiming.
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End,
	//	ObjectQueryParams);

	float Radius = 30.0f;
	TArray<FHitResult> Hits;
	FCollisionShape SphereShape;
	SphereShape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End,
		FQuat::Identity, ObjectQueryParams, SphereShape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	if (isDebugLinesEnabled)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0, 0, 2.0f);
	}

	// TODO: Maybe figure out a good way to determine a more intelligent way to assume what the user
	// might be trying to target.
	FHitResult* Hit = Hits.FindByPredicate(
		[OwningCharacter](const FHitResult& hit)
	{
		AActor* HitActor = hit.GetActor();
		return HitActor && HitActor->Implements<UARLGameplayInterface>();
	});

	if (!Hit)
	{
		return;
	}

	IARLGameplayInterface::Execute_Interact(Hit->GetActor(), Cast<APawn>(OwningCharacter));
	if (isDebugLinesEnabled)
	{
		DrawDebugSphere(GetWorld(), Hit->ImpactPoint, Radius, 32, LineColor, false, 2.0f);
	}
}

// Called when the game starts
void UARLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UARLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

