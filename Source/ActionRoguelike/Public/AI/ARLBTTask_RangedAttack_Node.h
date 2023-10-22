// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARLBTTask_RangedAttack_Node.generated.h"


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARLBTTask_RangedAttack_Node : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaximumBulletSpreadYaw = 2.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaximumBulletSpreadPitch = 2.0f;
};
