// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckTagFromASC.h"
#include "AIController.h"
#include "GameplayAbilities/Public/AbilitySystemComponent.h"

UBTDecorator_CheckTagFromASC::UBTDecorator_CheckTagFromASC()
{

}

bool UBTDecorator_CheckTagFromASC::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (ControlledPawn)
    {
        UAbilitySystemComponent* ASC = ControlledPawn->FindComponentByClass<UAbilitySystemComponent>();
        if (ASC)
        {
            return ASC->HasMatchingGameplayTag(TagToCheck);
        }
    }
    return false;
}
