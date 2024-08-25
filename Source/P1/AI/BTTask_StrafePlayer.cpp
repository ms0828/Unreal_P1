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
    float TimerInterval = 0.2f; // ������ ����
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
            // AI�� NewLocation�� ��ǥ�� �ε巴�� �̵��ϵ��� ��û
            AIController->MoveToLocation(NewLocation, 0.0f);
        }

        // ���Ͱ� �÷��̾ ��� �ٶ󺸰� ȸ��
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Monster->GetActorLocation(), PlayerLocation);
        Monster->SetActorRotation(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));
    }
    
}


