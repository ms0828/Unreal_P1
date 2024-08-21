// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/P1AnimNotify_AttackHitCheck.h"
#include "Interface/P1AttackAnimationInterface.h"
void UP1AnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		if (IP1AttackAnimationInterface* AttackPawn = Cast<IP1AttackAnimationInterface>(MeshComp->GetOwner()))
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
