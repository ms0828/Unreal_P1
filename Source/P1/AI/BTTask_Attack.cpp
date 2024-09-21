// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/P1Enemy.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/P1GameplayAbility_EnemyAttack.h"
#include "P1GameplayTags.h"

UBTTask_Attack::UBTTask_Attack()
{
	Tag = P1GameplayTags::AI_Enemy_AttackWaiting;
	CoolTime = 5.0f;
}



EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	OwnerCompCashed = &OwnerComp;

	AP1Enemy* Enemy = CastChecked<AP1Enemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(P1GameplayTags::AI_Enemy_AttackWaiting))
		{
			return EBTNodeResult::Succeeded;
		}

		FGameplayAbilitySpec* Spec = Enemy->AbilitySpecMap.Find(P1GameplayTags::Ability_Enemy_Attack);
		if (Spec)
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
		ASC->AddLooseGameplayTag(P1GameplayTags::AI_Enemy_AttackWaiting);
	}


	AttackCoolTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(AttackCoolTimerHandle, [ASC]() {
			ASC->RemoveLooseGameplayTag(P1GameplayTags::AI_Enemy_AttackWaiting);
		}, CoolTime, false);

	return EBTNodeResult::Succeeded;
}

