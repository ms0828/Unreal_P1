// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	None,
	Moving,
	Attack,
	Skill,
	Dead,
	Rolling,
};


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None,
	Moving,
	Attack,
	Skill,
	Dead,
	WatingAttack,
};

#define D(x) if(GEngine) { GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Cyan, x); }