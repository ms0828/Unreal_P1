// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckTagFromASC.generated.h"


UCLASS()
class P1_API UBTDecorator_CheckTagFromASC : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckTagFromASC();

	UPROPERTY(EditAnywhere, Category = "Gameplay Tags")
	FGameplayTag TagToCheck;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
