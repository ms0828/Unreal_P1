// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/P1GameplayAbility.h"
#include "P1GameplayAbility_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class P1_API UP1GameplayAbility_AttackHitCheck : public UP1GameplayAbility
{
	GENERATED_BODY()
	
public:
	UP1GameplayAbility_AttackHitCheck();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
