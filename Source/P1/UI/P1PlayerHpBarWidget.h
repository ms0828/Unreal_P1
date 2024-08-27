// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P1PlayerHpBarWidget.generated.h"


UCLASS()
class P1_API UP1PlayerHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	UP1PlayerHpBarWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetOwningPlayer(class AP1Player* InPlayer) { OwningPlayer = InPlayer; }

	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);


protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	TObjectPtr<class AP1Player> OwningPlayer;

	UPROPERTY()
	float MaxHp;

};
