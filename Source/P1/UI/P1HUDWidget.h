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
	void InitHpBar(float MaxHp);
	void UpdateHpBar(float NewCurrentHp);


protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UP1PlayerHpBarWidget> PlayerHpBar;


};
