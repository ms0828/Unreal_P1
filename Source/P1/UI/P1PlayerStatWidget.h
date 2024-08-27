// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/P1PlayerStat.h"
#include "P1PlayerStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class P1_API UP1PlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateStat(const FP1PlayerStat& BaseStat, const FP1PlayerStat& ModifierStat);
	
};
