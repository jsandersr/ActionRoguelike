// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UARLAction_ProjectileAttack::UARLAction_ProjectileAttack()
{
	HandSocketName = "Muzle_01";
	AnimDelaySeconds = 0.2f;
}

void UARLAction_ProjectileAttack::StartAction_Implementation(AActor* InstigatorActor)
{
	Super::StartAction_Implementation(InstigatorActor);

	ACharacter* Character = Cast<ACharacter>(InstigatorActor);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AnimDelaySeconds, false);
	}

}

void UARLAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (!ensureAlways(ProjectileClass))
	{
		return;
	}

	// This is so we don't have issues spawning inside of ourself.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorCharacter;

	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	//Ignore Player;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	// TODO: Will this call the derived one on AARLCharacter?
	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

	const float TraceLineLengthCM = 5000.0f;

	// Endpoint far into the look-at distance. Not too far, still adjust somewhat towards the
	// crosshair on a miss.
	// TODO: Figure out how to introduce accuracy variations.
	FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * TraceLineLengthCM);

	FHitResult HitResult;
	// Returns true if we got a blocking hit.
	if (GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd,
		FQuat::Identity, ObjParams, Shape, Params))
	{
		TraceEnd = HitResult.ImpactPoint;
	}

	// Now we trace from our hand location.
	TraceStart = InstigatorCharacter->GetMesh()->GetSocketLocation("Muzzle_01");

	// We will find a new direction/rotation that points from the hand to the impact point.
	FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();

	// Spawning is always done through the world.
	// Projectile class will be specified through a blueprint.
	FTransform SpawnTransformMatrix = FTransform{ ProjectileRotation, TraceStart };
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransformMatrix, SpawnParams);

	StopAction(InstigatorCharacter);
}
