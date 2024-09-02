// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "P1InputData.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT()
struct FP1InputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;
};

UCLASS()
class P1_API UP1InputData : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindInputActionByTag(const FGameplayTag& InputTag) const;

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TArray<FP1InputAction> InputActions;

};
