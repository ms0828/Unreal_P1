// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P1CommonMonsterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UP1CommonMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

class P1_API IP1CommonMonsterAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

public:
	virtual void AttackByAI() = 0;
};
