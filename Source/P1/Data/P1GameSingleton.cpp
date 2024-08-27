// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/P1GameSingleton.h"

DEFINE_LOG_CATEGORY(LogP1Singleton);

UP1GameSingleton::UP1GameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/P1PlayerStatTable.P1PlayerStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, PlayerStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FP1PlayerStat*>(Value);
			}
		);

		PlayerMaxLevel = PlayerStatTable.Num();
		ensure(PlayerMaxLevel > 0);
	}

	
}

UP1GameSingleton& UP1GameSingleton::Get()
{
	UP1GameSingleton* Singleton = CastChecked<UP1GameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogP1Singleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UP1GameSingleton>();
}
