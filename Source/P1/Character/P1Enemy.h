// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P1Character.h"
#include "P1Define.h"
#include "P1Enemy.generated.h"

/**
 * 
 */
UCLASS()
class P1_API AP1Enemy : public AP1Character
{
	GENERATED_BODY()
	
public:
	//virtual void OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker);
	//virtual void OnDead(TObjectPtr<AP1Character> Attacker);
	EEnemyState GetEnemyState();
	void SetEnemyState(EEnemyState InState);
	virtual void OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker) override;
	virtual void OnDead() override;

protected:
	UPROPERTY(BlueprintReadWrite)
	EEnemyState State = EEnemyState::None;


};
