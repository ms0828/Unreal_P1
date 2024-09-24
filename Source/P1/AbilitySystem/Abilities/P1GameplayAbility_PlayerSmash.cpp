
#include "AbilitySystem/Abilities/P1GameplayAbility_PlayerSmash.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/P1Player.h"
#include "GameFramework/CharacterMovementComponent.h"

UP1GameplayAbility_PlayerSmash::UP1GameplayAbility_PlayerSmash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP1GameplayAbility_PlayerSmash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AP1Player* Player = CastChecked<AP1Player>(ActorInfo->AvatarActor.Get());
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	UAbilityTask_PlayMontageAndWait* SmashAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlaySmash"), Player->GetSmashMontage(), 1.0f, GetNextSection());
	SmashAttackTask->OnCompleted.AddDynamic(this, &UP1GameplayAbility_PlayerSmash::OnCompletedCallback);
	SmashAttackTask->OnInterrupted.AddDynamic(this, &UP1GameplayAbility_PlayerSmash::OnInterruptedCallback);
	SmashAttackTask->OnCancelled.AddDynamic(this, &UP1GameplayAbility_PlayerSmash::OnInterruptedCallback);
	SmashAttackTask->ReadyForActivation();
}

void UP1GameplayAbility_PlayerSmash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AP1Player* Player = CastChecked<AP1Player>(CurrentActorInfo->AvatarActor.Get());
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Player->SetCurrentCombo(0);
}

void UP1GameplayAbility_PlayerSmash::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UP1GameplayAbility_PlayerSmash::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

FName UP1GameplayAbility_PlayerSmash::GetNextSection()
{
	AP1Player* Player = CastChecked<AP1Player>(CurrentActorInfo->AvatarActor.Get());
	FName NextSection = *FString::Printf(TEXT("%s%d"), TEXT("Smash"), Player->GetCurrentCombo());
	return NextSection;
}
