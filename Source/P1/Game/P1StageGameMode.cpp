// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/P1StageGameMode.h"
#include "Character/P1CommonMonster.h"
#include "Character/P1StageBoss.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Runtime/MovieScene/Public/MovieSceneSequencePlaybackSettings.h"
AP1StageGameMode::AP1StageGameMode() : Super()
{
	static ConstructorHelpers::FClassFinder<AP1CommonMonster> CommonMonsterClassRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_MonsterKhaimera.BP_MonsterKhaimera_C'"));
	if (CommonMonsterClassRef.Class)
	{
		MonsterClass = CommonMonsterClassRef.Class;
	}

    static ConstructorHelpers::FClassFinder<AP1StageBoss> StageBossClassRef(TEXT(""));
    if (StageBossClassRef.Class)
    {
        StageBossClass = StageBossClassRef.Class;
    }

    MonsterCount = 5;

    CommonMonsterSpawnLocations.Add(FVector(-1361.f, 2631.f, -440.f));
    CommonMonsterSpawnLocations.Add(FVector(-1169.f, 1110.f, -331.f));
    CommonMonsterSpawnLocations.Add(FVector(-2090.f, 1680.f, -411.f));
    CommonMonsterSpawnLocations.Add(FVector(-420.f, 1730.f, -431.f));
    CommonMonsterSpawnLocations.Add(FVector(100.f, 3530.f, -532.f));
}

void AP1StageGameMode::BeginPlay()
{
	Super::BeginPlay();

    SpawnCommonMonsters();
}

void AP1StageGameMode::SpawnCommonMonsters()
{
    if (MonsterClass != nullptr)
    {
        for (const FVector& SpawnLocation : CommonMonsterSpawnLocations)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;

            FRotator SpawnRotation = FRotator::ZeroRotator;

            AP1CommonMonster* SpawnMonster = GetWorld()->SpawnActor<AP1CommonMonster>(MonsterClass, SpawnLocation, SpawnRotation, SpawnParams);
            SpawnMonster->CommonMonsterDeadDelegate.AddUObject(this, &AP1StageGameMode::OnCommonMonsterKilled);
        }
    }
}

void AP1StageGameMode::OnCommonMonsterKilled()
{
    MonsterCount--;

    UE_LOG(LogTemp, Warning, TEXT("Monster Count : %d"), MonsterCount);

    if (MonsterCount == 0)
    {
        PlayBossSequence();
    }

}

void AP1StageGameMode::PlayBossSequence()
{
    FString SequencePath = TEXT("/Script/LevelSequence.LevelSequence'/Game/Sequence/BossSequence.BossSequence'");
    ULevelSequence* LevelSequence = Cast<ULevelSequence>(StaticLoadObject(ULevelSequence::StaticClass(), nullptr, *SequencePath));
    if (LevelSequence)
    {
        FMovieSceneSequencePlaybackSettings Settings;
        ALevelSequenceActor* OutActor = nullptr;
        ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, OutActor);

        if (SequencePlayer)
        {
            SequencePlayer->OnFinished.AddDynamic(this, &AP1StageGameMode::SpawnStageBoss);
            SequencePlayer->Play();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No SequencePlayer"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelSequence can't loaded"));
    }
}

void AP1StageGameMode::SpawnStageBoss()
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    FVector SpawnLocation = FVector(-1220.f, 160.f, -210.f);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    AP1StageBoss* SpawnBoss = GetWorld()->SpawnActor<AP1StageBoss>(StageBossClass, SpawnLocation, SpawnRotation, SpawnParams);
}



