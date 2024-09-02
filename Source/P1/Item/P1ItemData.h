// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "P1ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Potion,
	Scroll
};


UCLASS()
class P1_API UP1ItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;



};
