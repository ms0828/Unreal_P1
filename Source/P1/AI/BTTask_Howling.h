// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Howling.generated.h"

/**
 * 
 */
UCLASS()
class P1_API UBTTask_Howling : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Howling();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void HowlingMontageEnded(class UAnimMontage* Montage, bool bInterrupted, class UBehaviorTreeComponent& OwnerComp);
};
