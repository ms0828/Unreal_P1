// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "P1PlayerStat.h"
#include "P1GameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogP1Singleton, Error, All);

UCLASS()
class P1_API UP1GameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UP1GameSingleton();
	static UP1GameSingleton& Get();

public:
	FORCEINLINE FP1PlayerStat GetPlayerStat(int32 InLevel) const {
		return PlayerStatTable.IsValidIndex(InLevel) ? PlayerStatTable
			[InLevel] : FP1PlayerStat();
	};

	UPROPERTY()
	int32 PlayerMaxLevel;

private:
	TArray<FP1PlayerStat> PlayerStatTable;
};
