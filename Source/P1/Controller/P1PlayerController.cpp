// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/P1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Data/P1CharacterControlData.h"
#include "InputMappingContext.h"
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

	static ConstructorHelpers::FObjectFinder<UP1CharacterControlData> ShoulderDataRef(TEXT("/Script/P1.P1CharacterControlData'/Game/Data/Input/DA_ShoulderView.DA_ShoulderView'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
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

void AP1PlayerController::SetCharacterControlData(const UP1CharacterControlData* CharacterControlData)
{
	if (PossessedPawn == nullptr)
		return;

	PossessedPawn->bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	AP1Player* MyPlayer = Cast<AP1Player>(PossessedPawn);
	if (MyPlayer)
	{
		//CharacterMovement
		MyPlayer->GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationTMovement;
		MyPlayer->GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
		MyPlayer->GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
		
		//Player Component
		MyPlayer->GetCameraBoom()->TargetArmLength = CharacterControlData->TargetArmLength;
		MyPlayer->GetCameraBoom()->SetRelativeRotation(CharacterControlData->RelativeRotation);
		MyPlayer->GetCameraBoom()->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
		MyPlayer->GetCameraBoom()->bInheritPitch = CharacterControlData->bInheritPitch;
		MyPlayer->GetCameraBoom()->bInheritYaw = CharacterControlData->bInheritYaw;
		MyPlayer->GetCameraBoom()->bInheritRoll = CharacterControlData->bInheritRoll;
		MyPlayer->GetCameraBoom()->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
	}
	
}

void AP1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	PossessedPawn = GetPawn();

	if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Turn);
		EnhancedInputComponent->BindAction(ChangeViewPointAction, ETriggerEvent::Triggered, this, &ThisClass::Input_ChangeViewPointStart);
		EnhancedInputComponent->BindAction(ChangeViewPointAction, ETriggerEvent::Completed, this, &ThisClass::Input_ChangeViewPointEnd);
	}

	SetCharacterControlData(CharacterControlManager[ECharacterControlType::Shoulder]);
}

void AP1PlayerController::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	PossessedPawn->AddMovementInput(ForwardDirection, MovementVector.X);
	PossessedPawn->AddMovementInput(RightDirection, MovementVector.Y);
}

void AP1PlayerController::Input_Turn(const FInputActionValue& InputValue)
{
	if (IsChangeView)
	{
		FVector2D LookAxisVector = InputValue.Get<FVector2D>();
		PossessedPawn->AddControllerYawInput(LookAxisVector.X);
		PossessedPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AP1PlayerController::Input_ChangeViewPointStart(const FInputActionValue& InputValue)
{
	IsChangeView = true;
	UE_LOG(LogTemp, Log, TEXT("Start"));
}

void AP1PlayerController::Input_ChangeViewPointEnd(const FInputActionValue& InputValue)
{
	IsChangeView = false;
	UE_LOG(LogTemp, Log, TEXT("End"));
}
