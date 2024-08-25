// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RotateToPlayer.generated.h"


UCLASS()
class P1_API UBTService_RotateToPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_RotateToPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
