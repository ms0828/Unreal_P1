// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P1Character.h"
#include "P1Player.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class P1_API AP1Player : public AP1Character
{
	GENERATED_BODY()
	
public:
	AP1Player();

	USpringArmComponent* GetCameraBoom() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
};
