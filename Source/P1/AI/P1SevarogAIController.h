// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "P1SevarogAIController.generated.h"



UCLASS()
class P1_API AP1SevarogAIController : public AAIController
{
	GENERATED_BODY()

public:
	AP1SevarogAIController();

	void RunAI(class ACharacter* Player);
	void StopAI();
	TObjectPtr<class UBlackboardComponent> GetBlackboard();
	void SetWaitingAttackMode(bool InValue);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
