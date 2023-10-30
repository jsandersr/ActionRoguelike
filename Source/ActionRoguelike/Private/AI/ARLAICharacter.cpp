// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAICharacter.h"
#include "AIController.h"
#include "ARLAttributeComponent.h"
#include "ARLWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ARLActionComponent.h"

// Sets default values
AARLAICharacter::AARLAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComp = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UARLActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AARLAICharacter::ShowHealthBar()
{
	if (!ActiveHealthBar)
	{
		ActiveHealthBar = CreateWidget<UARLWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
	}
}

void AARLAICharacter::SetTargetActor(AActor* NewTarget)
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();

		BBComponent->SetValueAsObject("TargetActor", NewTarget);
	}
}

void AARLAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AARLAICharacter::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
	float NewHealth, float DeltaHealth)
{
	if (DeltaHealth < 0.0f)
	{
		// TODO: Don't target the instigator if it's not the player.
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		ShowHealthBar();

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Died.
		if (NewHealth <= 0.0f)
		{
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Dead");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");


			SetLifeSpan(10.0f);

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
		}
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC)
		{
			UBlackboardComponent* BBComponent = AIC->GetBlackboardComponent();
			BBComponent->SetValueAsBool("IsDead", !OwningComp->IsAlive());
		}
	}
}

void AARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AARLAICharacter::OnPawnSeen);
	AttributeComp->HealthChangedSignal.AddDynamic(this, &AARLAICharacter::OnHealthChanged);
}

void AARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}
