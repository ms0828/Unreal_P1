// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1CommonMonster.h"
#include "P1Define.h"
#include "Physics/P1Collision.h"
#include "Components/CapsuleComponent.h"

AP1CommonMonster::AP1CommonMonster()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_P1CAPSULE);
}

void AP1CommonMonster::OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker)
{
	Hp = FMath::Clamp(Hp - Damage, 0, MaxHp);
	if (Hp == 0)
	{
		OnDead(Attacker);
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
}
