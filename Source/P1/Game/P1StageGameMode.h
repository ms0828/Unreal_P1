
#pragma once

#include "CoreMinimal.h"
#include "Game/P1GameMode.h"
#include "P1StageGameMode.generated.h"



UCLASS()
class P1_API AP1StageGameMode : public AP1GameMode
{
	GENERATED_BODY()
	
public:
	AP1StageGameMode();

protected:
	void BeginPlay() override;

	void SpawnCommonMonsters();

	void OnCommonMonsterKilled();

	void PlayBossSequence();

	UFUNCTION()
	void SpawnStageBoss();

private:
	int32 MonsterCount;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<class AP1CommonMonster> MonsterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<class AP1StageBoss> StageBossClass;

	UPROPERTY()
	TArray<FVector> CommonMonsterSpawnLocations;


	

};
