// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P1Enemy.h"
#include "P1CommonMonster.generated.h"

UCLASS()
class P1_API AP1CommonMonster : public AP1Enemy
{
	GENERATED_BODY()
	
public:
	AP1CommonMonster();

public:
	virtual void OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker) override;
	virtual void OnDead(TObjectPtr<AP1Character> Attacker) override;

};
