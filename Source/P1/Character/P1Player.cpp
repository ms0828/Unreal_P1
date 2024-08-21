// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1Player.h"
#include "Controller/P1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Data/P1ComboAttackData.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AP1Player::AP1Player()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstaceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Player.ABP_Player_C'"));
	if (AnimInstaceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstaceClassRef.Class);
	}

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

USpringArmComponent* AP1Player::GetCameraBoom() const
{
	return CameraBoom;
}

void AP1Player::Input_Move(const FInputActionValue& InputValue)
{
	if (isRolling)
	{
		return;
	}

	FVector2D MovementVector = InputValue.Get<FVector2D>();
	CurrentMoveInputDirection = MovementVector;
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AP1Player::Released_Move(const FInputActionValue& InputValue)
{
	CurrentMoveInputDirection = FVector2D::ZeroVector;
}

void AP1Player::ProcessComboAttack()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	if (isRolling)
	{
		return;
	}
	if (PlayerController == nullptr)
	{
		PlayerController = Cast<AP1PlayerController>(GetController());
	}

	if (CurrentCombo == 0)
	{
		FRotator ControllerRotation = PlayerController->GetControlRotation();
		ControllerRotation.Pitch = 0;
		ControllerRotation.Roll = 0;
		SetActorRotation(ControllerRotation);

		ComboAttackBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboAttack = false;
	}
	else
	{
		HasNextComboAttack = true;
	}
}

void AP1Player::ComboAttackBegin()
{
	CurrentCombo = 1;
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboAttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AP1Player::ComboAttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboAttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AP1Player::ComboAttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	CurrentCombo = 0;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AP1Player::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboAttackData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = (ComboAttackData->EffectiveFrameCount[ComboIndex] / ComboAttackData->FrameRate) / AttackSpeedRate;

	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AP1Player::ComboCheck, ComboEffectiveTime, false);
	}
}

void AP1Player::ComboCheck()
{
	ComboTimerHandle.Invalidate();	//init
	if (HasNextComboAttack)
	{
		UAnimInstance* AnimInstace = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboAttackData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboAttackData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstace->Montage_JumpToSection(NextSection, ComboAttackMontage);
		SetComboCheckTimer();
		HasNextComboAttack = false;
	}
}

void AP1Player::ProcessRolling()
{
	if (PlayerController == nullptr)
	{
		PlayerController = Cast<AP1PlayerController>(GetController());
	}
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	if (isRolling)
	{
		return;
	}
	if (CurrentMoveInputDirection == FVector2D::ZeroVector)
	{
		return;
	}


	FRotator ControlRotation = PlayerController->GetControlRotation();
	FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	// 카메라 기준 앞쪽(Forward)과 오른쪽(Right) 벡터 계산
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // X축 = 앞쪽
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);   // Y축 = 오른쪽

	// 입력 값을 기준으로 월드 좌표계 방향 벡터 계산
	FVector MoveDirection = (ForwardDirection * CurrentMoveInputDirection.X) + (RightDirection * CurrentMoveInputDirection.Y);

	// 이동 방향을 정규화
	MoveDirection.Normalize();
	FRotator TargetRotation = MoveDirection.Rotation();

	SetActorRotation(TargetRotation);
	RollingBegin(MoveDirection);
}


void AP1Player::RollingBegin(FVector Direction)
{
	isRolling = true;
	CurrentCombo = 0;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(ComboAttackMontage))
	{
		AnimInstance->Montage_Stop(0.2f);
	}
	AnimInstance->Montage_Play(RollingMontage, RollingSpeedRate);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);


	//float RollDistance = 1000.0f; // 구르기 동안 이동할 거리
	//float RollDuration = RollingMontage->GetPlayLength(); // 몽타주의 재생 시간
	//FVector RollVelocity = Direction * RollDistance / RollDuration; // 속도 계산
	//LaunchCharacter(RollVelocity, true, true); // 캐릭터를 해당 방향으로 발사

	//FVector RollImpulse = Direction * RollDistance;
	//GetCharacterMovement()->AddImpulse(RollImpulse, true);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AP1Player::RollingEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RollingMontage);
}

void AP1Player::RollingEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	CurrentCombo = 0;
	isRolling = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
