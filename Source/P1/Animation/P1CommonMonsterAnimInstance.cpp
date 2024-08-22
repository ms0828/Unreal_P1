// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/P1CommonMonsterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UP1CommonMonsterAnimInstance::UP1CommonMonsterAnimInstance()
{
	MovingThreshold = 3.0f;
}

void UP1CommonMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();


	if (Owner = Cast<ACharacter>(GetOwningActor()))
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UP1CommonMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
