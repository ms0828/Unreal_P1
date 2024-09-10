// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/P1AnimNotify_AttackHitCheck_GAS.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "P1GameplayTags.h"

UP1AnimNotify_AttackHitCheck_GAS::UP1AnimNotify_AttackHitCheck_GAS()
{
	TriggerGameplayTag = P1GameplayTags::Character_Action_AttackHitCheck;
}

FString UP1AnimNotify_AttackHitCheck_GAS::GetNotifyName_Implementation() const
{
	return TEXT("AttackHitCheck_GAS");
}

void UP1AnimNotify_AttackHitCheck_GAS::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;

			// OwnerActor must have ASC
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
