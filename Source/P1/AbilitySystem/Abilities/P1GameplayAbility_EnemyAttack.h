// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/P1GameplayAbility.h"
#include "P1GameplayAbility_EnemyAttack.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnAbilityCompletedDelegate);

UCLASS()
class P1_API UP1GameplayAbility_EnemyAttack : public UP1GameplayAbility
{
	GENERATED_BODY()

public:
	UP1GameplayAbility_EnemyAttack();


public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	UFUNCTION()
	void OnCompletedCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	FName GetNextSection();

	TObjectPtr<class UAnimMontage> AttackMontage;

protected:
	uint8 MaxComboCount = 0;
	uint8 CurrentCombo = 0;

};
