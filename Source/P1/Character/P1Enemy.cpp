// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1Enemy.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "AbilitySystem/P1EnemyAttributeSet.h"
#include "AbilitySystem/Abilities/P1GameplayAbility.h"

AP1Enemy::AP1Enemy()
{
	ASC = CreateDefaultSubobject<UP1AbilitySystemComponent>("AbilitySystemComponent");
	
	AttributeSet = CreateDefaultSubobject<UP1EnemyAttributeSet>("EnemyAttributeSet");
}

void AP1Enemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystem();
}

EEnemyState AP1Enemy::GetEnemyState()
{
	return State;
}

void AP1Enemy::SetEnemyState(EEnemyState InState)
{
	this->State = InState;
}

void AP1Enemy::OnDamaged()
{

}

void AP1Enemy::OnDead()
{

}

TObjectPtr<class UAnimMontage> AP1Enemy::GetAttackMontage()
{
	return AttackMontage;
}

void AP1Enemy::InitAbilitySystem()
{
	Super::InitAbilitySystem();

	ASC->InitAbilityActorInfo(this, this);
	
	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility.Ability);
		AbilitySpecMap.Add(StartAbility.Tag, StartSpec);
		ASC->GiveAbility(StartSpec);
	}
}
