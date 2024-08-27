// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P1PlayerHpBarWidget.h"
#include "Components/ProgressBar.h"

// NativeConstruct가 호출될 때는, 거의 모든 UI 관련 기능이 초기화 되는 시점
void UP1PlayerHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName("PbHpBar"));
	ensure(HpProgressBar);
}

UP1PlayerHpBarWidget::UP1PlayerHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UP1PlayerHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	ensure(HpProgressBar);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
