// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P1PlayerController.generated.h"

struct FInputActionValue;

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,

};

UCLASS()
class P1_API AP1PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AP1PlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	//void SetCharacterControlData(const class UP1CharacterControlData* CharacterControlData);

private:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Turn(const FInputActionValue& InputValue);
	void Input_Attack(const FInputActionValue& InputValue);



protected:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> TurnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY()
	TObjectPtr<class APawn> PossessedPawn;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UP1CharacterControlData*> CharacterControlManager;

private:

};
