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

public:
	void SetupGASInputComponent(class UP1AbilitySystemComponent* ASC);

private:
	void Input_Turn(const FInputActionValue& InputValue);


protected:

	UPROPERTY()
	TObjectPtr<class APawn> PossessedPawn;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UP1CharacterControlData*> CharacterControlManager;


	// HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UP1HUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UP1HUDWidget> HUDWidget;
};
