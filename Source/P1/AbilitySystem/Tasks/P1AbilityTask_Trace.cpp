// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/P1AbilityTask_Trace.h"
#include "AbilitySystem/TargetActors/P1TargetActor_Trace.h"
#include "AbilitySystemComponent.h"
UP1AbilityTask_Trace::UP1AbilityTask_Trace()
{

}

UP1AbilityTask_Trace* UP1AbilityTask_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AP1TargetActor_Trace> TargetActorClass)
{
	UP1AbilityTask_Trace* NewTask = NewAbilityTask< UP1AbilityTask_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UP1AbilityTask_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UP1AbilityTask_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UP1AbilityTask_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AP1TargetActor_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UP1AbilityTask_Trace::OnTargetDataReadyCallback);
	}
}

void UP1AbilityTask_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargetingAndContinue();
	}
}

void UP1AbilityTask_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
