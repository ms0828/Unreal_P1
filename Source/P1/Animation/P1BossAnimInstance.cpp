// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/P1BossAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UP1BossAnimInstance::UP1BossAnimInstance()
{
	MovingThreshold = 3.0f;
}

void UP1BossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Owner = Cast<ACharacter>(GetOwningActor()))
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UP1BossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
	}
}
