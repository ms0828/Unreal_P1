// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/P1AnimNotify_SendGameplayEvent.h"
UP1AnimNotify_SendGameplayEvent::UP1AnimNotify_SendGameplayEvent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UP1AnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	/*if (AP1Player* LocalCharacter = Cast<AP1Player>(MeshComp->GetOwner()))
	{
		LocalCharacter->HandleGameplayEvent(EventTag);
	}*/


}
