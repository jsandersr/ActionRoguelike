// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARLBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
public:


protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AttackRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
