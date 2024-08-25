// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "P1AI.h"
#include "AIController.h"
#include "Interface/P1CommonMonsterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IP1CommonMonsterAIInterface* AIPawn = Cast<IP1CommonMonsterAIInterface>(ControllingPawn);

	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->AttackByAI();
	return EBTNodeResult::Succeeded;
}
