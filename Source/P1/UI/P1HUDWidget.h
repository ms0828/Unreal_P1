// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/P1PlayerStat.h"
#include "P1HUDWidget.generated.h"


UCLASS()
class P1_API UP1HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UP1HUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateHpBar(float NewCurrentHp);
	void UpdateStat(const FP1PlayerStat& BaseStat, const FP1PlayerStat& ModifierStat);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UP1PlayerHpBarWidget> PlayerHpBar;

	UPROPERTY()
	TObjectPtr<class UP1PlayerStatWidget> PlayerStat;
};
