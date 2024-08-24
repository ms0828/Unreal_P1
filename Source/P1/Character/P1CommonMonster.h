// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P1Enemy.h"
#include "Interface/P1CommonMonsterAIInterface.h"
#include "P1CommonMonster.generated.h"

UCLASS()
class P1_API AP1CommonMonster : public AP1Enemy, public IP1CommonMonsterAIInterface
{
	GENERATED_BODY()
	
public:
	AP1CommonMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	float AttackCoolTime = 5.0f;

	FTimerHandle AttackCoolTimerHandle;
	FTimerHandle AttackFinishedHandle;

public:
	virtual void OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker) override;
	virtual void OnDead(TObjectPtr<AP1Character> Attacker) override;
	virtual void EnableRagdoll() override;
	virtual void Disappear() override;

protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void AttackByAI() override;
	void AttackCoolTimeInit();
	void AttackFinished();
};
