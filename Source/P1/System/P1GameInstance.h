// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P1GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class P1_API UP1GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UP1GameInstance(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	virtual void Shutdown() override;
	
};
