// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARLInteractionComponent.h"
#include "ARLAttributeComponent.h"

// Sets default values
AARLCharacter::AARLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UARLInteractionComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called every frame
void AARLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARLCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AARLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AARLCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

// Called when the game starts or when spawned
void AARLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARLCharacter::MoveForward(float Value)
{
	// We want to move in the direction of the camera. Ex. Camera is looking to the left, rotate in that direction.
	// For more info look up ControlRotation vs CharacterRotation.
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void AARLCharacter::MoveRight(float Value)
{
	// Let the camera determine movement.
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Unreal Coordinate system:
	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	// We got this from FVector UKismetMathLibrary::GetRightVector(FRotator InRot).
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AARLCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
		&AARLCharacter::PrimaryAttack_TimeElapsed, PrimaryAttackDelaySeconds, false);

	// This is how you'd stop in-flight events from happening. You need its handle.
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack)
}

void AARLCharacter::PrimaryAttack_TimeElapsed()
{
	// Control location is "where are we looking"
	//FTransform SpawnTransformMatrix = FTransform{ GetControlRotation(), HandLocation };

	// This is so we don't have issues spawning inside of ourself.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	//Ignore Player;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	FVector TraceStart = CameraComp->GetComponentLocation();

	const float TraceLineLengthCM = 5000.0f;
	// Endpoint far into the look-at distance. Not too far, still adjust somewhat towards the
	// crosshair on a miss.

	// TODO: Figure out how to introduce accuracy variations.
	FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * TraceLineLengthCM);

	FHitResult HitResult;
	// Returns true if we got a blocking hit.
	if (GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd,
		FQuat::Identity, ObjParams, Shape, Params))
	{
		TraceEnd = HitResult.ImpactPoint;
	}

	// Now we trace from our hand location.
	TraceStart = GetMesh()->GetSocketLocation("Muzzle_01");

	// We will find a new direction/rotation that points from the hand to the impact point.
	FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - TraceStart).Rotator();

	// Spawning is always done through the world.
	// Projectile class will be specified through a blueprint.
	FTransform SpawnTransformMatrix = FTransform{ ProjectileRotation, TraceStart };
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransformMatrix, SpawnParams);
}

void AARLCharacter::PrimaryInteract()
{
	ensureAlways(InteractionComp);
	InteractionComp->PrimaryInteract();
}
