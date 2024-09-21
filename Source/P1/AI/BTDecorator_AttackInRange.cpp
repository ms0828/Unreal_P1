// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "P1AI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystem/P1AttributeSet.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "Character/P1Character.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AP1Character* ControllingPawn = Cast<AP1Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	UP1AbilitySystemComponent* ASC = Cast<UP1AbilitySystemComponent>(ControllingPawn->GetAbilitySystemComponent());
	if (ASC == nullptr)
	{
		return false;
	}

	const UP1AttributeSet* AttributeSet = ASC->GetSet<UP1AttributeSet>();
	if (AttributeSet == nullptr)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target == nullptr)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeRadius = AttributeSet->GetAttackRange();
	bResult = (DistanceToTarget <= AttackRangeRadius);

	return bResult;
}
