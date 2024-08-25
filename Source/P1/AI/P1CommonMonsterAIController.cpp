// Fill out your copyright notice in the Description page of Project Settings.


#include "P1CommonMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/P1CommonMonsterAnimInstance.h"
#include "GameFramework/Character.h"
#include "AI/P1AI.h"

AP1CommonMonsterAIController::AP1CommonMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_CommonMonster.BB_CommonMonster'"));
	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_CommonMonster.BT_CommonMonster'"));
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AP1CommonMonsterAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AP1CommonMonsterAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

TObjectPtr<class UBlackboardComponent> AP1CommonMonsterAIController::GetBlackboard()
{
	return Blackboard.Get();
}

void AP1CommonMonsterAIController::SetWaitingAttackMode(bool InValue)
{
	Blackboard->SetValueAsBool(BBKEY_ISWAITINGATTACK, InValue);
}

void AP1CommonMonsterAIController::SetStrafePlayerMode(bool InValue)
{
	ACharacter* Monster = Cast<ACharacter>(GetPawn());
	if (Monster == nullptr)
	{
		return;
	}
	UP1CommonMonsterAnimInstance* MonsterAnimInstance = Cast<UP1CommonMonsterAnimInstance>(Monster->GetMesh()->GetAnimInstance());
	MonsterAnimInstance->SetStrafePlayerMode(InValue);
}

void AP1CommonMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
