// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/P1GameplayAbility_PlayerRolling.h"
#include "Character/P1Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"

UP1GameplayAbility_PlayerRolling::UP1GameplayAbility_PlayerRolling()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UP1GameplayAbility_PlayerRolling::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;
	}

	const AP1Player* MyPlayer = Cast<AP1Player>(ActorInfo->AvatarActor.Get());
	return (MyPlayer && MyPlayer->CurrentMoveInputDirection != FVector2D::ZeroVector);
}


void UP1GameplayAbility_PlayerRolling::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AP1Player* Player = CastChecked<AP1Player>(ActorInfo->AvatarActor.Get());
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(Player->GetComboAttackMontage()))
	{
		AnimInstance->Montage_Stop(0.2f);
	}

	FRotator ControlRotation = Player->GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	// 카메라 기준 앞쪽(Forward)과 오른쪽(Right) 벡터 계산
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // X축 = 앞쪽
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);   // Y축 = 오른쪽

	// 입력 값을 기준으로 월드 좌표계 방향 벡터 계산
	FVector MoveDirection = (ForwardDirection * Player->CurrentMoveInputDirection.X) + (RightDirection * Player->CurrentMoveInputDirection.Y);

	// 이동 방향을 정규화
	MoveDirection.Normalize();
	FRotator TargetRotation = MoveDirection.Rotation();

	Player->SetActorRotation(TargetRotation);

	UAbilityTask_PlayMontageAndWait* PlayRollingTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayRolling"), Player->GetRollingMontage(), 1.0f);
	PlayRollingTask->OnCompleted.AddDynamic(this, &UP1GameplayAbility_PlayerRolling::OnCompletedCallback);
	PlayRollingTask->OnInterrupted.AddDynamic(this, &UP1GameplayAbility_PlayerRolling::OnInterruptedCallback);
	PlayRollingTask->OnCancelled.AddDynamic(this, &UP1GameplayAbility_PlayerRolling::OnInterruptedCallback);
	PlayRollingTask->OnBlendOut.AddDynamic(this, &UP1GameplayAbility_PlayerRolling::OnInterruptedCallback);
	PlayRollingTask->ReadyForActivation();
}

void UP1GameplayAbility_PlayerRolling::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	AP1Player* Player = CastChecked<AP1Player>(ActorInfo->AvatarActor.Get());
}

void UP1GameplayAbility_PlayerRolling::OnCompletedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UP1GameplayAbility_PlayerRolling::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}


