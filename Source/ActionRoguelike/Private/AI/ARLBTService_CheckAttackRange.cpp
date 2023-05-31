// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UARLBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	// Check the distance between the AI and the player
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ensure(BlackboardComp);

	// Get the AI pawn
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (!TargetActor || !ensure(AIController))
	{
		return;
	}

	APawn* AIPawn = AIController->GetPawn();
	ensure(AIPawn);
	float Distance = FVector::Dist(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

	bool bHasLos = false;
	bool bIsInAttackRange = Distance <= 2000.0f;
	if (bIsInAttackRange)
	{
		bHasLos = AIController->LineOfSightTo(TargetActor);
	}

	BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bIsInAttackRange && bHasLos);
}
