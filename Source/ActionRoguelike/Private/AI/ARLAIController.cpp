// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AARLAIController::BeginPlay()
{
	Super::BeginPlay();

	ensure(AIBehaviorTree);

	RunBehaviorTree(AIBehaviorTree);

	// PLACEHOLDER: This is not what you would normally do.
	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}
}
