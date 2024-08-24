// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "P1CommonMonsterAIController.generated.h"


UCLASS()
class P1_API AP1CommonMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AP1CommonMonsterAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
