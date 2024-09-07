// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/P1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Data/P1CharacterControlData.h"
#include "InputMappingContext.h"
#include "Character/P1Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/P1HUDWidget.h"
#include "System/P1AssetManager.h"
#include "Data/P1InputData.h"
#include "P1GameplayTags.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"

AP1PlayerController::AP1PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UP1HUDWidget> HUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_P1HUD.WBP_P1HUD_C'"));
	if (HUDWidgetRef.Succeeded())
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}
}

void AP1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (const UP1InputData* InputData = UP1AssetManager::GetAssetByName<UP1InputData>("InputData"))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputData->InputMappingContext, 0);
		}
	}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UP1HUDWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
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
	
	if (const UP1InputData* InputData = UP1AssetManager::GetAssetByName<UP1InputData>("InputData"))
	{
		if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			auto MoveAction = InputData->FindInputActionByTag(P1GameplayTags::Input_Action_Move);
			auto TurnAction = InputData->FindInputActionByTag(P1GameplayTags::Input_Action_Turn);

			EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Turn);
			if (AP1Player* MyPlayer = Cast<AP1Player>(PossessedPawn))
			{
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, MyPlayer, &AP1Player::Input_Move);
				EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, MyPlayer, &AP1Player::Released_Move);
			}
		}
	}

}

void AP1PlayerController::SetupGASInputComponent(UP1AbilitySystemComponent* ASC)
{
	if (IsValid(ASC))
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		PossessedPawn = GetPawn();

		if (const UP1InputData* InputData = UP1AssetManager::GetAssetByName<UP1InputData>("InputData"))
		{
			auto AttackAction = InputData->FindInputActionByTag(P1GameplayTags::Input_Action_Attack);
			auto RollingAction = InputData->FindInputActionByTag(P1GameplayTags::Input_Action_Rolling);

			if (AP1Player* MyPlayer = Cast<AP1Player>(PossessedPawn))
			{
				EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, MyPlayer, &AP1Player::GasInputPressed, 0);
				EnhancedInputComponent->BindAction(RollingAction, ETriggerEvent::Triggered, MyPlayer, &AP1Player::GasInputPressed, 1);
			}
		}
	}
}



void AP1PlayerController::Input_Turn(const FInputActionValue& InputValue)
{
	FVector2D LookAxisVector = InputValue.Get<FVector2D>();
	PossessedPawn->AddControllerYawInput(LookAxisVector.X * 0.8f);
	PossessedPawn->AddControllerPitchInput(LookAxisVector.Y * 0.8f);
}
