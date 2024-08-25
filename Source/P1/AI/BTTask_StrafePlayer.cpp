// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_StrafePlayer.h"
#include "P1AI.h"
#include "AI/P1CommonMonsterAIController.h"
#include "NavigationSystem.h"
#include "Interface/P1CommonMonsterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
UBTTask_StrafePlayer::UBTTask_StrafePlayer()
{
}

EBTNodeResult::Type UBTTask_StrafePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AP1CommonMonsterAIController* AIController = Cast<AP1CommonMonsterAIController>(OwnerComp.GetAIOwner());
    if (AIController == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    APawn* Monster = AIController->GetPawn();
    if (Monster == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AIController->SetStrafePlayerMode(true);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    APawn* Player = Cast<APawn>(BlackboardComp->GetValueAsObject(BBKEY_TARGET));
    if (Player == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    FVector PlayerLocation = Player->GetActorLocation();
    FVector MonsterLocation = Monster->GetActorLocation();
    float Radius = 300.0f;
        //FVector::Dist(PlayerLocation, MonsterLocation);
    
    StartStrafeTimer(OwnerComp, Monster, Player, Radius);

    return EBTNodeResult::InProgress;
}


void UBTTask_StrafePlayer::StartStrafeTimer(UBehaviorTreeComponent& OwnerComp, APawn* Monster, APawn* Player, float Radius)
{
    float TimerInterval = 0.2f; // 일정한 간격
    StrafeTimerHandle.Invalidate();
    GetWorld()->GetTimerManager().SetTimer(StrafeTimerHandle, FTimerDelegate::CreateUObject(
        this, &UBTTask_StrafePlayer::StrafeAroundPlayer, Monster, Player, Radius), TimerInterval, true);


    float FinishDelay = FMath::FRandRange(2.0f, 3.0f);
    GetWorld()->GetTimerManager().SetTimer(FinishTaskTimerHandle, FTimerDelegate::CreateLambda([this, &OwnerComp]()
        {
            GetWorld()->GetTimerManager().ClearTimer(StrafeTimerHandle);
            AP1CommonMonsterAIController* AIController = Cast<AP1CommonMonsterAIController>(OwnerComp.GetAIOwner());
            if (AIController)
            {
                AIController->SetStrafePlayerMode(false);
            }
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }), FinishDelay, false);
}



void UBTTask_StrafePlayer::StrafeAroundPlayer(APawn* Monster, APawn* Player, float Radius)
{
    if (Monster && Player)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector MonsterLocation = Monster->GetActorLocation();

        // 플레이어를 기준으로 회전할 벡터 계산
        FVector Direction = MonsterLocation - PlayerLocation;
        Direction.Z = 0; // 높이 차이를 무시하여 2D 평면상에서만 계산
        Direction.Normalize(); // 방향을 단위 벡터로 변환

        // 회전 각도 계산
        float AngularSpeed = 200.0f; // 초당 회전할 각도 (도 단위)
        float AngleDelta = -AngularSpeed * GetWorld()->GetTimerManager().GetTimerElapsed(StrafeTimerHandle); // 현재 프레임에서 회전할 각도

        // 회전 변환 행렬을 사용하여 새로운 방향을 계산 (시계 방향 혹은 반시계 방향으로 회전)
        FRotator RotationDelta(0.0f, AngleDelta, 0.0f); // Yaw 축 기준 회전
        FVector NewDirection = RotationDelta.RotateVector(Direction);

        // 새로운 위치 계산 (원둘레를 따라 이동)
        FVector NewLocation = PlayerLocation + NewDirection * Radius;

        // 몬스터를 새로운 위치로 이동
        AP1CommonMonsterAIController* AIController = Cast<AP1CommonMonsterAIController>(Monster->GetController());
        if (AIController)
        {
            // AI가 NewLocation을 목표로 부드럽게 이동하도록 요청
            AIController->MoveToLocation(NewLocation, 0.0f);
        }

        // 몬스터가 플레이어를 계속 바라보게 회전
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Monster->GetActorLocation(), PlayerLocation);
        Monster->SetActorRotation(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));
    }
    
}


