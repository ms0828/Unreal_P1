// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/P1GameplayAbility.h"
#include "P1GameplayAbility_PlayerAttack.generated.h"


UCLASS()
class P1_API UP1GameplayAbility_PlayerAttack : public UP1GameplayAbility
{
	GENERATED_BODY()
	
public:
	UP1GameplayAbility_PlayerAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

protected:
	UFUNCTION()
	void OnCompletedCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	FName GetNextSection();

	void StartComboTimer();
	void CheckComboInput();

protected:
	UPROPERTY()
	TObjectPtr<class UP1ComboAttackData> ComboAttackData;

	uint8 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;
};
