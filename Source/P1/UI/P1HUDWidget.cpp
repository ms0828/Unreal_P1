// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P1HUDWidget.h"
#include "UI/P1PlayerHpBarWidget.h"
#include "Character/P1Player.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "AbilitySystem/P1AttributeSet.h"

UP1HUDWidget::UP1HUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UP1HUDWidget::InitHpBar(float NewMaxHp)
{
	PlayerHpBar->SetMaxHp(NewMaxHp);
}

void UP1HUDWidget::UpdateHpBar(float CurrentHp)
{
	PlayerHpBar->UpdateHpBar(CurrentHp);
}

void UP1HUDWidget::SetAbilitySystemComponent(TObjectPtr<UP1AbilitySystemComponent> NewASC)
{
	if (NewASC)
	{
		ASC = NewASC;
		ASC->GetGameplayAttributeValueChangeDelegate(UP1AttributeSet::GetHpAttribute()).AddUObject(this, &UP1HUDWidget::OnHpChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UP1AttributeSet::GetMaxHpAttribute()).AddUObject(this, &UP1HUDWidget::OnMaxHpChanged);
	}
}


void UP1HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerHpBar = Cast<UP1PlayerHpBarWidget>(GetWidgetFromName(TEXT("PlayerHpBarWidget")));
	ensure(PlayerHpBar);
	
	AP1Player* Player = Cast<AP1Player>(GetOwningPlayerPawn());
	if (Player)
	{
		Player->SetupHUDWidget(this);
	}
}

void UP1HUDWidget::OnHpChanged(const FOnAttributeChangeData& ChangeData)
{
	UpdateHpBar(ChangeData.NewValue);
}

void UP1HUDWidget::OnMaxHpChanged(const FOnAttributeChangeData& ChangeData)
{
	InitHpBar(ChangeData.NewValue);
}
