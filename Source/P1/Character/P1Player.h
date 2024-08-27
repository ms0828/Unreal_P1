// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P1Character.h"
#include "P1Define.h"
#include "Interface/P1AttackAnimationInterface.h"
#include "P1Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class AP1PlayerController;

UCLASS()
class P1_API AP1Player : public AP1Character, public IP1AttackAnimationInterface
{
	GENERATED_BODY()
	
public:
	AP1Player();

	USpringArmComponent* GetCameraBoom() const;


protected:
	virtual void BeginPlay() override;
	void ComboAttackBegin();
	void ComboAttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();
	void RollingBegin(FVector Direction);
	void RollingEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void AttackHitCheck() override;

	// ---ui
	void SetupPlayerHpBarWidget();

public:
	void ProcessComboAttack();
	void ProcessRolling();
	void Input_Move(const FInputActionValue& InputValue);
	void Released_Move(const FInputActionValue& InputValue);
	EPlayerState GetPlayerState();
	void SetPlayerState(EPlayerState InState);
	virtual void OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker) override;
	virtual void OnDead() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RollingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attackm, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP1ComboAttackData> ComboAttackData;

	const float AttackSpeedRate = 1.2f;
	const float RollingSpeedRate = 1.0f;
	int32 CurrentCombo = 0;		//'0' means combo do not start 
	FTimerHandle ComboTimerHandle;
	bool HasNextComboAttack = false;


	AP1PlayerController* PlayerController;
	FVector2D CurrentMoveInputDirection;

	UPROPERTY(BlueprintReadWrite)
	EPlayerState State = EPlayerState::None;


	//----UI

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UP1PlayerHpBarWidget> HpBarWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UP1PlayerHpBarWidget> HpBarWidget;
};
