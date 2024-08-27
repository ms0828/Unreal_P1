// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P1CharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P1_API UP1CharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UP1CharacterStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }
	float ApplyDamage(float InDamage);

	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;


protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float AttackDamage;
		
	void SetHp(float NewHp);
	void SetAttackDamage(float Value);

};
