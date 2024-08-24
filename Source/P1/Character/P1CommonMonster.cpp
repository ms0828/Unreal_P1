// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1CommonMonster.h"
#include "P1Define.h"
#include "Physics/P1Collision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/P1CommonMonsterAIController.h"
#include "Components/CapsuleComponent.h"

AP1CommonMonster::AP1CommonMonster()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_P1CAPSULE);
    AIControllerClass = AP1CommonMonsterAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


void AP1CommonMonster::OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker)
{
	Hp = FMath::Clamp(Hp - Damage, 0, MaxHp);
	if (Hp == 0)
	{
		OnDead(Attacker);
        return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (HitReactionMontage)
	{
        // Convert Yaw -> -180 to 180
        float CharacterYaw = FMath::UnwindDegrees(GetActorRotation().Yaw);
        float AttackerYaw = FMath::UnwindDegrees(Attacker->GetActorRotation().Yaw);
        float YawDifference = AttackerYaw - CharacterYaw;

        // Normalize yaw difference
        if (YawDifference > 180.0f) YawDifference -= 360.0f;
        if (YawDifference < -180.0f) YawDifference += 360.0f;

        FName SectionName;
        

        if (YawDifference > -45.0f && YawDifference < 45.0f)
        {
            SectionName = TEXT("Back");
        }
        else if (YawDifference > 135.0f || YawDifference < -135.0f)
        {
            SectionName = TEXT("Front");
        }
        else if (YawDifference > 45.0f && YawDifference < 135.0f)
        {
            SectionName = TEXT("Left");
        }
        else
        {
            SectionName = TEXT("Right");
        }

		AnimInstance->Montage_Play(HitReactionMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactionMontage);
	}
	
	D(FString::Printf(TEXT("%d"), Hp));
}

void AP1CommonMonster::OnDead(TObjectPtr<AP1Character> Attacker)
{
	if (State == EEnemyState::Dead)
	{
		return;
	}
	State = EEnemyState::Dead;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->StopAllMontages(0.0f);
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

    if (DeadAnimSequence)
    {
        GetMesh()->PlayAnimation(DeadAnimSequence, false);
        DeadAnimationTimerHandle.Invalidate();
        GetWorld()->GetTimerManager().SetTimer(DeadAnimationTimerHandle, this, &AP1CommonMonster::Disappear, DeadAnimSequence->GetPlayLength(), false);
    }
}

void AP1CommonMonster::Disappear()
{
    SetLifeSpan(1.0f);
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
    if (State == EEnemyState::StandByAttack)
    {
        return;
    }

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(AttackMontage, 1.0f);
    State = EEnemyState::StandByAttack;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    AttackCoolTimerHandle.Invalidate();
    GetWorld()->GetTimerManager().SetTimer(AttackCoolTimerHandle, this, &AP1CommonMonster::AttackCoolTimeInit, AttackCoolTime, false);

    AttackFinishedHandle.Invalidate();
    GetWorld()->GetTimerManager().SetTimer(AttackFinishedHandle, this, &AP1CommonMonster::AttackFinished, AttackMontage->GetPlayLength(), false);
}

void AP1CommonMonster::AttackCoolTimeInit()
{
    State = EEnemyState::None;
}

void AP1CommonMonster::AttackFinished()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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


