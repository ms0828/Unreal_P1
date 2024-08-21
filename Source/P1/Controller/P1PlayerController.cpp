// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/P1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Data/P1CharacterControlData.h"
#include "InputMappingContext.h"
#include "Character/P1Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/P1Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
AP1PlayerController::AP1PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultIMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Data/Input/IMC_Player.IMC_Player'"));
	if (DefaultIMCRef.Object)
	{
		InputMappingContext = DefaultIMCRef.Object;
	}
}

void AP1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}


void AP1PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		SetupInputComponent();
	}
}


void AP1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	PossessedPawn = GetPawn();
	
	if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Turn);
		if (AP1Player* MyPlayer = Cast<AP1Player>(PossessedPawn))
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, MyPlayer, &AP1Player::Input_Move);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, MyPlayer, &AP1Player::Released_Move);
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, MyPlayer, &AP1Player::ProcessComboAttack);
			EnhancedInputComponent->BindAction(RollingAction, ETriggerEvent::Triggered, MyPlayer, &AP1Player::ProcessRolling);
		}
	}
}

void AP1PlayerController::Input_Turn(const FInputActionValue& InputValue)
{
	FVector2D LookAxisVector = InputValue.Get<FVector2D>();
	PossessedPawn->AddControllerYawInput(LookAxisVector.X * 0.8f);
	PossessedPawn->AddControllerPitchInput(LookAxisVector.Y * 0.8f);
}
