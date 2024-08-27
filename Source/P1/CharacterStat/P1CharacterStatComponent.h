// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/P1PlayerStat.h"
#include "P1CharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FP1PlayerStat&, const FP1PlayerStat&);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P1_API UP1CharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UP1CharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }

	FORCEINLINE void SetBaseStat(const FP1PlayerStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FP1PlayerStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE FP1PlayerStat GetBaseStat() const { return BaseStat; }
	FORCEINLINE FP1PlayerStat GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FP1PlayerStat GetTotalStat() const { return BaseStat + ModifierStat; }

	void SetHp(float NewHp);
	void SetLevelStat(int32 InNewLevel);
	
	float ApplyDamage(float InDamage);


	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

protected:

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FP1PlayerStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FP1PlayerStat ModifierStat;

};
