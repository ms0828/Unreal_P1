// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "P1ComboAttackData.generated.h"

/**
 * 
 */
UCLASS()
class P1_API UP1ComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UP1ComboAttackData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	//Frame Play Speed
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = Name)
	TArray<float> EffectiveFrameCount;
};
