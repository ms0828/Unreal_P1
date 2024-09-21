// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/P1SevarogAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "AI/P1AI.h"
#include "Kismet/GameplayStatics.h"

AP1SevarogAIController::AP1SevarogAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Boss_Sevarog.BB_Boss_Sevarog'"));
	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Boss_Sevarog.BT_Boss_Sevarog'"));
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AP1SevarogAIController::RunAI(ACharacter* Player)
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsObject(BBKEY_TARGET, Player);
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AP1SevarogAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}



TObjectPtr<class UBlackboardComponent> AP1SevarogAIController::GetBlackboard()
{
	return Blackboard.Get();
}

void AP1SevarogAIController::SetWaitingAttackMode(bool InValue)
{
	Blackboard->SetValueAsBool(BBKEY_ISWAITINGATTACK, InValue);
}

void AP1SevarogAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("OnPossess!"));
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerCharacter!"));
		RunAI(PlayerCharacter);
	}
	
}
