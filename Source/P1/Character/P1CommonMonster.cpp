// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1CommonMonster.h"
#include "P1Define.h"
#include "Physics/P1Collision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/P1CommonMonsterAIController.h"
#include "Components/CapsuleComponent.h"
#include "Character/P1Player.h"
#include "AbilitySystem/P1EnemyAttributeSet.h"
#include "GameplayTagContainer.h"
#include "P1GameplayTags.h"

AP1CommonMonster::AP1CommonMonster()
{
    AIControllerClass = AP1CommonMonsterAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


void AP1CommonMonster::BeginPlay()
{
    Super::BeginPlay();

    FGameplayTag CommonMonsterTag = P1GameplayTags::Character_Identity_CommonMonster;
    GameplayTagContainer.AddTag(CommonMonsterTag);
}

void AP1CommonMonster::OnDamaged()
{
	if (AttributeSet->GetHp() == 0)
	{
		OnDead();
        return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (HitReactionMontage)
	{
		AnimInstance->Montage_Play(HitReactionMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(TEXT("Front"), HitReactionMontage);
	}
	
	D(FString::Printf(TEXT("%d"), AttributeSet->GetHp()));
}

void AP1CommonMonster::OnDead()
{
	if (State == EEnemyState::Dead)
	{
		return;
	}

    CommonMonsterDeadDelegate.Broadcast();

	State = EEnemyState::Dead;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->StopAllMontages(0.0f);
    
    if (DeadAnimMontage)
    {
        AnimInstance->Montage_Play(DeadAnimMontage, 1.0f);
        DeadAnimationTimerHandle.Invalidate();
        GetWorld()->GetTimerManager().SetTimer(DeadAnimationTimerHandle, this, &AP1CommonMonster::Disappear, DeadAnimMontage->GetPlayLength() * 0.7f, false);
    }
}

void AP1CommonMonster::Disappear()
{
    SetLifeSpan(0.1f);
}

float AP1CommonMonster::GetAIPatrolRadius()
{
    return 800.0f;
}

float AP1CommonMonster::GetAIDetectRange()
{
    return 700.0f;
}

float AP1CommonMonster::GetAIAttackRange()
{
    return 70.0f;
}

float AP1CommonMonster::GetAITurnSpeed()
{
    return 0.0f;
}

void AP1CommonMonster::AttackByAI()
{
    if (State == EEnemyState::WatingAttack || State == EEnemyState::Dead)
    {
        return;
    }

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(AttackMontage, 1.0f);
    State = EEnemyState::WatingAttack;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    AttackCoolTimerHandle.Invalidate();
    GetWorld()->GetTimerManager().SetTimer(AttackCoolTimerHandle, this, &AP1CommonMonster::AttackCoolTimeInit, AttackCoolTime, false);

    AttackFinishedHandle.Invalidate();
    GetWorld()->GetTimerManager().SetTimer(AttackFinishedHandle, this, &AP1CommonMonster::AttackFinished, AttackMontage->GetPlayLength(), false);
}



TObjectPtr<UAnimMontage> AP1CommonMonster::GetPatrollingMontage()
{
    if (PatrollingMontage == nullptr)
    {
        return nullptr;
    }
    return PatrollingMontage;
}

TObjectPtr<UAnimMontage> AP1CommonMonster::GetHowlingMontage()
{
    if (HowlingMontage == nullptr)
    {
        return nullptr;
    }
    return HowlingMontage;
}


void AP1CommonMonster::AttackCoolTimeInit()
{
    State = EEnemyState::None;
    AP1CommonMonsterAIController* AIController = Cast<AP1CommonMonsterAIController>(GetController());
    AIController->SetWaitingAttackMode(false);
}

void AP1CommonMonster::AttackFinished()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    AP1CommonMonsterAIController* AIController = Cast<AP1CommonMonsterAIController>(GetController());
    AIController->SetWaitingAttackMode(true);
}




void AP1CommonMonster::EnableRagdoll()
{
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->GravityScale = 0.0f;
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
    GetMesh()->SetEnableGravity(true);

    GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector, false);
    GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector, false);

    GetMesh()->SetSimulatePhysics(true);
    SetLifeSpan(10.0f);
}


