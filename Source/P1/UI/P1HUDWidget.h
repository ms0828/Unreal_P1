// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "P1HUDWidget.generated.h"


UCLASS()
class P1_API UP1HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UP1HUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void InitHpBar(float NewMaxHp);
	void UpdateHpBar(float CurrentHp);

	void SetAbilitySystemComponent(TObjectPtr<class UP1AbilitySystemComponent> NewASC);

protected:
	virtual void NativeConstruct() override;

	void OnHpChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxHpChanged(const FOnAttributeChangeData& ChangeData);

protected:
	UPROPERTY()
	TObjectPtr<class UP1PlayerHpBarWidget> PlayerHpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UP1AbilitySystemComponent> ASC;

};
