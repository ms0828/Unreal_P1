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

    TWeakObjectPtr<APawn> WeakPtrMonster = AIController->GetPawn();
    if (!WeakPtrMonster.IsValid())
    {
        return EBTNodeResult::Failed;
    }

    AIController->SetStrafePlayerMode(true);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    TWeakObjectPtr<APawn> WeakPtrPlayer = Cast<APawn>(BlackboardComp->GetValueAsObject(BBKEY_TARGET));
    if (!WeakPtrPlayer.IsValid())
    {
        return EBTNodeResult::Failed;
    }

    FVector PlayerLocation = WeakPtrPlayer.Get()->GetActorLocation();
    FVector MonsterLocation = WeakPtrMonster.Get()->GetActorLocation();
    float Radius = 300.0f;

    StrafeTimerHandle.Invalidate();
    FinishTaskTimerHandle.Invalidate();
    StartStrafeTimer(OwnerComp, WeakPtrMonster, WeakPtrPlayer, Radius);

    return EBTNodeResult::InProgress;
}


void UBTTask_StrafePlayer::StartStrafeTimer(UBehaviorTreeComponent& OwnerComp, TWeakObjectPtr<APawn> Monster, TWeakObjectPtr<APawn> Player, float Radius)
{
    float TimerInterval = 0.2f; // ������ ����
    
    GetWorld()->GetTimerManager().SetTimer(StrafeTimerHandle, FTimerDelegate::CreateLambda([Monster, Player, this, &OwnerComp, Radius]()
        {
            if (!Monster.IsValid() || !Player.IsValid())
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }
            StrafeAroundPlayer(Monster, Player, Radius);
        }), TimerInterval, true);

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



void UBTTask_StrafePlayer::StrafeAroundPlayer(TWeakObjectPtr<APawn> Monster, TWeakObjectPtr<APawn> Player, float Radius)
{
    if (Monster.IsValid() && Player.IsValid())
    {
        FVector PlayerLocation = Player->GetActorLocation();
        FVector MonsterLocation = Monster->GetActorLocation();

        // �÷��̾ �������� ȸ���� ���� ���
        FVector Direction = MonsterLocation - PlayerLocation;
        Direction.Z = 0; // ���� ���̸� �����Ͽ� 2D ���󿡼��� ���
        Direction.Normalize(); // ������ ���� ���ͷ� ��ȯ

        // ȸ�� ���� ���
        float AngularSpeed = 200.0f; // �ʴ� ȸ���� ���� (�� ����)
        float AngleDelta = -AngularSpeed * GetWorld()->GetTimerManager().GetTimerElapsed(StrafeTimerHandle); // ���� �����ӿ��� ȸ���� ����

        // ȸ�� ��ȯ ����� ����Ͽ� ���ο� ������ ��� (�ð� ���� Ȥ�� �ݽð� �������� ȸ��)
        FRotator RotationDelta(0.0f, AngleDelta, 0.0f); // Yaw �� ���� ȸ��
        FVector NewDirection = RotationDelta.RotateVector(Direction);

        // ���ο� ��ġ ��� (���ѷ��� ���� �̵�)
        FVector NewLocation = PlayerLocation + NewDirection * Radius;

        // ���͸� ���ο� ��ġ�� �̵�
        AP1CommonMonsterAIController* AIController = Cast<AP1CommonMonsterAIController>(Monster->GetController());
        if (AIController)
        {
            AIController->MoveToLocation(NewLocation, 0.0f);
        }
    }
    
}


