// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P1Enemy.h"
#include "Interface/P1CommonMonsterAIInterface.h"
#include "P1CommonMonster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCommonMonsterDead);

UCLASS()
class P1_API AP1CommonMonster : public AP1Enemy, public IP1CommonMonsterAIInterface
{
	GENERATED_BODY()
	
public:
	AP1CommonMonster();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> PatrollingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> HowlingMontage;

	float AttackCoolTime = 7.0f;
	FTimerHandle AttackCoolTimerHandle;
	FTimerHandle AttackFinishedHandle;


public:
	virtual void BeginPlay() override;

	virtual void OnDamaged() override;

	virtual void OnDead() override;

	virtual void EnableRagdoll() override;
	virtual void Disappear() override;

public:
	FOnCommonMonsterDead CommonMonsterDeadDelegate;

public:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	virtual void AttackByAI() override;


	TObjectPtr<UAnimMontage> GetPatrollingMontage();
	TObjectPtr<UAnimMontage> GetHowlingMontage();

	void AttackCoolTimeInit();
	void AttackFinished();
};
