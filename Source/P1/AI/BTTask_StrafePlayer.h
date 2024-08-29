// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StrafePlayer.generated.h"

/**
 * 
 */
UCLASS()
class P1_API UBTTask_StrafePlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_StrafePlayer();
protected:
    FTimerHandle StrafeTimerHandle;
    FTimerHandle FinishTaskTimerHandle;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    void StartStrafeTimer(UBehaviorTreeComponent& OwnerComp, TWeakObjectPtr<APawn> Monster, TWeakObjectPtr<APawn> Player, float Radius);
    void StrafeAroundPlayer(TWeakObjectPtr<APawn> Monster, TWeakObjectPtr<APawn> Player, float Radius);
};
