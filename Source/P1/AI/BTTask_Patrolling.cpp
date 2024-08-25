// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Patrolling.h"
#include "P1AI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/P1CommonMonster.h"
#include "Animation/AnimMontage.h"

UBTTask_Patrolling::UBTTask_Patrolling()
{

}

EBTNodeResult::Type UBTTask_Patrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AP1CommonMonster* ControllingMonster = Cast<AP1CommonMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingMonster == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = ControllingMonster->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AnimInstance->Montage_Play(ControllingMonster->GetPatrollingMontage(), 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this, &OwnerComp](UAnimMontage* Montage, bool bInterrupted)
		{
			PatrolMontageEnded(Montage, bInterrupted, OwnerComp);
		});
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ControllingMonster->GetPatrollingMontage());

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Patrolling::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	AP1CommonMonster* ControllingMonster = Cast<AP1CommonMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingMonster == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = ControllingMonster->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AnimInstance->StopAllMontages(0.0f);

	return EBTNodeResult::Type();
}

void UBTTask_Patrolling::PatrolMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent& OwnerComp)
{
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
