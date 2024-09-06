// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1Enemy.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "AbilitySystem/P1EnemyAttributeSet.h"

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

void AP1Enemy::OnDamaged(float Damage, TObjectPtr<AP1Character> Attacker)
{

}

void AP1Enemy::OnDead()
{

}

void AP1Enemy::InitAbilitySystem()
{
	Super::InitAbilitySystem();

	ASC->InitAbilityActorInfo(this, this);
}
