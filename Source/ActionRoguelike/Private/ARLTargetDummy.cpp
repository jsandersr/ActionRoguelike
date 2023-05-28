// Fill out your copyright notice in the Description page of Project Settings.


#include "ARLTargetDummy.h"

#include "Components/StaticMeshComponent.h"
#include "ARLAttributeComponent.h"

// Sets default values
AARLTargetDummy::AARLTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UARLAttributeComponent>("AttributeComp");
}

void AARLTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->HealthChangedSignal.AddDynamic(this, &AARLTargetDummy::OnHealthChanged);
}

void AARLTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARLAttributeComponent* OwningComp,
	float NewHealth, float DeltaHealth)
{
	if (DeltaHealth < 0)
	{
		MeshComp->SetScalarParameterValueOnMaterials("HitTimestamp", GetWorld()->TimeSeconds);
	}
}
