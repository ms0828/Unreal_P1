// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/P1PlayerAttributeSet.h"
#include "P1PlayerState.generated.h"


UCLASS()
class P1_API AP1PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AP1PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UP1AbilitySystemComponent* GetP1AbilitySystemComponent() const;
	UP1PlayerAttributeSet* GetPlayerAttributeSet() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UP1AbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UP1PlayerAttributeSet> PlayerAttributeSet;
};
