// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Patrolling.generated.h"



UCLASS()
class P1_API UBTTask_Patrolling : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Patrolling();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void PatrolMontageEnded(class UAnimMontage* Montage, bool bInterrupted, class UBehaviorTreeComponent& OwnerComp);
	
};
