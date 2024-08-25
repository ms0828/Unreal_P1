// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_RotateToPlayer.h"
#include "P1AI.h"
#include "AIController.h"
#include "Character/P1Character.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTService_RotateToPlayer::UBTService_RotateToPlayer()
{

}

void UBTService_RotateToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACharacter* ControllingCharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingCharacter == nullptr)
	{
		return;
	}
	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	FVector Direction = Target->GetActorLocation() - ControllingCharacter->GetActorLocation();
	FRotator TargetRotation = Direction.Rotation();
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;
	ControllingCharacter->SetActorRotation(TargetRotation);
}
