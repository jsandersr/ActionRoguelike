// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLInteractionComponent.h"
#include "ARLGameplayInterface.h"
#include "ARLWorldUserWidget.h"

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
	// Key Note: We need to pass the actor in as a parameter to
	// interact with UEs replication system. Let UE figure out whose focused actor this is.
	ServerInteract(FocusedActor);
}

// Called every frame
void UARLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* PlayerPawn = Cast<APawn>(GetOwner());
	if (PlayerPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UARLInteractionComponent::FindBestInteractable()
{
	bool isDebugLinesEnabled = CVarDrawDebugLinesEnabledInteraction.GetValueOnGameThread();
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* OwningCharacter = GetOwner();

	FRotator EyeRotation;
	FVector EyeLocation;
	OwningCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	// You could detect hit with a simple ray cast. But this requires the player be pixel perfect with their
	// aiming.
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End,
	//	ObjectQueryParams);

	TArray<FHitResult> Hits;
	FCollisionShape SphereShape;
	SphereShape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End,
		FQuat::Identity, ObjectQueryParams, SphereShape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	if (isDebugLinesEnabled)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0, 0, 0.0f);
	}

	FocusedActor = nullptr;
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

	FocusedActor = Hit->GetActor();
	if (FocusedActor)
	{
		if (!IsValid(DefaultWidgetInstance) && ensure(IsValid(DefaultWidgetClass)))
		{
			DefaultWidgetInstance = CreateWidget<UARLWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (IsValid(DefaultWidgetInstance))
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (isDebugLinesEnabled)
	{
		DrawDebugSphere(GetWorld(), Hit->ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
	}
}

void UARLInteractionComponent::ServerInteract_Implementation(AActor* CurrentFocusActor)
{
	if (!IsValid(CurrentFocusActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact");
		return;
	}

	AActor* OwningCharacter = GetOwner();

	// Key Note:
	// The syntax here is just part of the UE domain knowledge. "Its just how it works".
	// Execute_<FunctionName>(<Actor that this should get called on>, <Param list>)
	IARLGameplayInterface::Execute_Interact(CurrentFocusActor, Cast<APawn>(OwningCharacter));
}

