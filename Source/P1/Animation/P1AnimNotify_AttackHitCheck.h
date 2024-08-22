// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "P1AnimNotify_AttackHitCheck.generated.h"


UCLASS()
class P1_API UP1AnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
