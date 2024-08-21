// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/P1AttackAnimationInterface.h"
#include "P1Character.generated.h"

UCLASS()
class P1_API AP1Character : public ACharacter, public IP1AttackAnimationInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AP1Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void AttackHitCheck() override;
};
