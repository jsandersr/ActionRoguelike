// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARLBTTask_RangedAttack_Node.h"
#include "AIController.h"
#include "ARLAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


EBTNodeResult::Type UARLBTTask_RangedAttack_Node::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* AIPawn = AIController->GetCharacter();
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_01");

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	if (!UARLAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	MuzzleRotation.Pitch += FMath::RandRange(0.f, MaximumBulletSpreadPitch);
	MuzzleRotation.Yaw += FMath::RandRange(0.f, MaximumBulletSpreadYaw);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = AIPawn;

	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
	if (!NewProjectile)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
