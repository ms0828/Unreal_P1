// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1Enemy.h"

EEnemyState AP1Enemy::GetEnemyState()
{
	return State;
}

void AP1Enemy::SetEnemyState(EEnemyState InState)
{
	this->State = InState;
}

void AP1Enemy::OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker)
{

}

void AP1Enemy::OnDead()
{

}
