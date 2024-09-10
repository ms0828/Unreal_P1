// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "P1AnimNotify_AttackHitCheck_GAS.generated.h"

/**
 * 
 */
UCLASS()
class P1_API UP1AnimNotify_AttackHitCheck_GAS : public UAnimNotify
{
	GENERATED_BODY()

public:
	UP1AnimNotify_AttackHitCheck_GAS();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY()
	FGameplayTag TriggerGameplayTag;
	
};
