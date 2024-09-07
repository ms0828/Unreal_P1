// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/P1GameplayAbility_PlayerAttack.h"
#include "Character/P1Player.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/P1ComboAttackData.h"

UP1GameplayAbility_PlayerAttack::UP1GameplayAbility_PlayerAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP1GameplayAbility_PlayerAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AP1Player* Player = CastChecked<AP1Player>(ActorInfo->AvatarActor.Get());
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	ComboAttackData = Player->GetComboAttackData();
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), Player->GetComboAttackMontage(), 1.0f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UP1GameplayAbility_PlayerAttack::OnCompletedCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UP1GameplayAbility_PlayerAttack::OnInterruptedCallback);
	PlayAttackTask->OnCancelled.AddDynamic(this, &UP1GameplayAbility_PlayerAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
	StartComboTimer();
}

void UP1GameplayAbility_PlayerAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AP1Player* Player = CastChecked<AP1Player>(ActorInfo->AvatarActor.Get());
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	ComboAttackData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;
}


void UP1GameplayAbility_PlayerAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
	}
}

void UP1GameplayAbility_PlayerAttack::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UP1GameplayAbility_PlayerAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}



FName UP1GameplayAbility_PlayerAttack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboAttackData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboAttackData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UP1GameplayAbility_PlayerAttack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboAttackData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = ComboAttackData->EffectiveFrameCount[ComboIndex] / ComboAttackData->FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UP1GameplayAbility_PlayerAttack::CheckComboInput, ComboEffectiveTime, false);
	}

}

void UP1GameplayAbility_PlayerAttack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}
