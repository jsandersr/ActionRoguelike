// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARLInteractionComponent.h"
#include "ARLAttributeComponent.h"
#include "ARLActionComponent.h"

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
	ActionComp = CreateDefaultSubobject<UARLActionComponent>("ActionComp");

	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called to bind functionality to input
void AARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARLCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AARLCharacter::UsePrimaryInteract);

	PlayerInputComponent->BindAction("PrimaryAbility", IE_Pressed, this, &AARLCharacter::UsePrimaryAbility);
	PlayerInputComponent->BindAction("TeleportAbility", IE_Pressed, this, &AARLCharacter::UseTeleportAbility);
	PlayerInputComponent->BindAction("BlackHoleAbility", IE_Pressed, this, &AARLCharacter::UseBlackHoleAbility);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AARLCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AARLCharacter::SprintStop);
}

void AARLCharacter::HealSelf(float Amount /*= 100*/)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void AARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->HealthChangedSignal.AddDynamic(this, &AARLCharacter::OnHealthChanged);
}

FVector AARLCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
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

void AARLCharacter::UsePrimaryAbility()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");

	// This is how you'd stop in-flight events from happening. You need its handle.
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack)
}

void AARLCharacter::UseTeleportAbility()
{
	ActionComp->StartActionByName(this, "Dash");
}

void AARLCharacter::UseBlackHoleAbility()
{
	ActionComp->StartActionByName(this, "BlackHole");
}

void AARLCharacter::UsePrimaryInteract()
{
	ensureAlways(InteractionComp);
	InteractionComp->PrimaryInteract();
}

void AARLCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AARLCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AARLCharacter::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp, float NewHealth, float DeltaHealth)
{
	if (NewHealth <= 0.0f && DeltaHealth < 0.0f)
	{
		APlayerController* PlayerController =  Cast<APlayerController>(GetController());
		DisableInput(PlayerController);

		SetLifeSpan(10.0f);
	}
}
