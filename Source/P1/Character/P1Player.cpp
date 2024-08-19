// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1Player.h"
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

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstaceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/ABP_Player.ABP_Player_C'"));
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
	ensure(CurrentCombo != 0);
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

void AP1Player::ProcessComboAttack()
{
	if (CurrentCombo == 0)
	{
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
