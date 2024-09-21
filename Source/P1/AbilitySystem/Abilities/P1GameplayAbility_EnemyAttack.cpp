// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/P1GameplayAbility_EnemyAttack.h"
#include "Character/P1Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/BTTask_Attack.h"

UP1GameplayAbility_EnemyAttack::UP1GameplayAbility_EnemyAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP1GameplayAbility_EnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AP1Enemy* Enemy = CastChecked<AP1Enemy>(ActorInfo->AvatarActor.Get());
	Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	AttackMontage = Enemy->GetAttackMontage();
	if (AttackMontage)
	{
		MaxComboCount = AttackMontage->GetNumSections();
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Enemy->GetAttackMontage(), 1.0f, GetNextSection());
		PlayAttackTask->OnCompleted.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnInterruptedCallback);
		PlayAttackTask->OnCancelled.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnInterruptedCallback);
		PlayAttackTask->ReadyForActivation();
	}
}

void UP1GameplayAbility_EnemyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentCombo = 0;

	AP1Enemy* Enemy = CastChecked<AP1Enemy>(ActorInfo->AvatarActor.Get());
	Enemy->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UP1GameplayAbility_EnemyAttack::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;

	if (CurrentCombo == MaxComboCount)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
	}
	else
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage, 1.0f, GetNextSection());
		PlayAttackTask->OnCompleted.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnInterruptedCallback);
		PlayAttackTask->OnCancelled.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnInterruptedCallback);
		PlayAttackTask->ReadyForActivation();
	}
	
}

void UP1GameplayAbility_EnemyAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;

	if (CurrentCombo == MaxComboCount)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
	}
	else
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), AttackMontage, 1.0f, GetNextSection());
		PlayAttackTask->OnCompleted.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnInterruptedCallback);
		PlayAttackTask->OnCancelled.AddDynamic(this, &UP1GameplayAbility_EnemyAttack::OnInterruptedCallback);
		PlayAttackTask->ReadyForActivation();
	}
}

FName UP1GameplayAbility_EnemyAttack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), TEXT("Attack"), CurrentCombo);
	return NextSection;
}
