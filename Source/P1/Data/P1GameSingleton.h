// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "P1GameSingleton.generated.h"


UCLASS()
class P1_API UP1GameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UP1GameSingleton();
	static UP1GameSingleton& Get();

public:


};
