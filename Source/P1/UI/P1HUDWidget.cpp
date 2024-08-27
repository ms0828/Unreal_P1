// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P1HUDWidget.h"
#include "UI/P1PlayerStatWidget.h"
#include "UI/P1PlayerHpBarWidget.h"
#include "Character/P1Player.h"
#include "Data/P1PlayerStat.h"

UP1HUDWidget::UP1HUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UP1HUDWidget::UpdateHpBar(float NewCurrentHp)
{
	PlayerHpBar->UpdateHpBar(NewCurrentHp);
}

void UP1HUDWidget::UpdateStat(const FP1PlayerStat& BaseStat, const FP1PlayerStat& ModifierStat)
{
	FP1PlayerStat TotalStat = BaseStat + ModifierStat;
	PlayerHpBar->SetMaxHp(TotalStat.MaxHp);
	PlayerStat->UpdateStat(BaseStat, ModifierStat);
}

void UP1HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerHpBar = Cast<UP1PlayerHpBarWidget>(GetWidgetFromName(TEXT("PlayerHpBarWidget")));
	ensure(PlayerHpBar);

	PlayerStat = Cast<UP1PlayerStatWidget>(GetWidgetFromName(TEXT("PlayerStatWidget")));
	ensure(PlayerStat);
	
	AP1Player* Player = Cast<AP1Player>(GetOwningPlayerPawn());
	if (Player)
	{
		Player->SetupHUDWidget(this);
	}
}
