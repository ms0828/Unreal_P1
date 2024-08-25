// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Howling.h"
#include "P1AI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/P1CommonMonster.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Howling::UBTTask_Howling()
{
}

EBTNodeResult::Type UBTTask_Howling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AP1CommonMonster* ControllingMonster = Cast<AP1CommonMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingMonster == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = ControllingMonster->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return EBTNodeResult::Failed;
	}


	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	FVector Direction = Target->GetActorLocation() - ControllingMonster->GetActorLocation();

	FRotator TargetRotation = Direction.Rotation();
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;
	ControllingMonster->SetActorRotation(TargetRotation);


	ControllingMonster->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	AnimInstance->Montage_Play(ControllingMonster->GetHowlingMontage(), 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			HowlingMontageEnded(Montage, bInterrupted, OwnerComp);
		});
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ControllingMonster->GetHowlingMontage());

	return EBTNodeResult::InProgress;
}


void UBTTask_Howling::HowlingMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent& OwnerComp)
{
	ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
