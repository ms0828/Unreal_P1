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
	AP1Enemy();

protected:
	virtual void BeginPlay() override;

public:
	EEnemyState GetEnemyState();
	void SetEnemyState(EEnemyState InState);
	virtual void OnDamaged() override;
	virtual void OnDead() override;

//GAS
public:
	virtual void InitAbilitySystem() override;

protected:
	UPROPERTY(BlueprintReadWrite)
	EEnemyState State = EEnemyState::None;


	//Gas
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TArray<TSubclassOf<class UP1GameplayAbility>> StartAbilities;
};
