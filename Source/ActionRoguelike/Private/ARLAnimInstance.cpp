#include "ARLAnimInstance.h"
#include "GameplayTagContainer.h"

#include "ARLActionComponent.h"

void UARLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (IsValid(OwningActor))
	{
		ActionComp = Cast<UARLActionComponent>(
			OwningActor->GetComponentByClass(UARLActionComponent::StaticClass()));
	}
}

void UARLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if (IsValid(ActionComp))
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}

}
