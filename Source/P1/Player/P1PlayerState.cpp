// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/P1PlayerState.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "AbilitySystem/P1PlayerAttributeSet.h"

AP1PlayerState::AP1PlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ASC = CreateDefaultSubobject<UP1AbilitySystemComponent>("AbilitySystemComponent");
	PlayerAttributeSet = CreateDefaultSubobject<UP1PlayerAttributeSet>("PlayerAttributeSet");
}

UAbilitySystemComponent* AP1PlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

UP1AbilitySystemComponent* AP1PlayerState::GetP1AbilitySystemComponent() const
{
	return ASC;
}

UP1PlayerAttributeSet* AP1PlayerState::GetPlayerAttributeSet() const
{
	return PlayerAttributeSet;
}
