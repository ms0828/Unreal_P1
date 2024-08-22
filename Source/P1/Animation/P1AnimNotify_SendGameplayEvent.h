// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "P1AnimNotify_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class P1_API UP1AnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UP1AnimNotify_SendGameplayEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;

};
