// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P1PlayerItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UP1PlayerItemInterface : public UInterface
{
	GENERATED_BODY()
};


class P1_API IP1PlayerItemInterface
{
	GENERATED_BODY()

public:
	virtual void TakeItem(class UP1ItemData* InItemData) = 0;
};
