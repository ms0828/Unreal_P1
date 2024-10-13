// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/P1GameplayAbility_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Tasks/P1AbilityTask_Trace.h"
#include "AbilitySystem/TargetActors/P1TargetActor_Trace.h"
#include "P1GameplayTags.h"
#include "Character/P1Character.h"
#include "System/P1AssetManager.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "AbilitySystem/P1AttributeSet.h"

UP1GameplayAbility_AttackHitCheck::UP1GameplayAbility_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UP1GameplayAbility_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ComboAttackLevel = TriggerEventData->EventMagnitude;
	AbilityTag = TriggerEventData->InstigatorTags.First();
	DamageTable = UP1AssetManager::GetAssetByName<UCurveTable>("DamageTable");
	DamageCurve = static_cast<FSimpleCurve*>(DamageTable->FindCurve(*AbilityTag.ToString(), TEXT("DamageTable Context")));
	UP1AbilityTask_Trace* AttackTraceTask = UP1AbilityTask_Trace::CreateTask(this, AP1TargetActor_Trace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UP1GameplayAbility_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();}

void UP1GameplayAbility_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, ComboAttackLevel);

		float Coefficient = DamageCurve->Eval(ComboAttackLevel, 1);
		
		AP1Character* Character = Cast<AP1Character>(GetAvatarActorFromActorInfo());
		UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
		const UP1AttributeSet* AttributeSet = Cast<UP1AttributeSet>(ASC->GetAttributeSet(UP1AttributeSet::StaticClass()));
		float AttackRate = AttributeSet->GetAttackRate();
		float FinalValue = AttackRate * Coefficient;

		// Todo
		// - normal or cirtical
		// not completed
		EffectSpecHandle.Data->SetSetByCallerMagnitude(P1GameplayTags::Data_Damage_Normal, FinalValue);


		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
