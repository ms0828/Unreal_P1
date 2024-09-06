// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1Player.h"
#include "Controller/P1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Data/P1ComboAttackData.h"
#include "P1Define.h"
#include "Physics/P1Collision.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/P1CharacterStatComponent.h"
#include "UI/P1HUDWidget.h"
#include "Item/P1ItemData.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "AbilitySystem/P1PlayerAttributeSet.h"
#include "Player/P1PlayerState.h"

AP1Player::AP1Player()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstaceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/Player/ABP_Player.ABP_Player_C'"));
	if (AnimInstaceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstaceClassRef.Class);
	}

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_P1CAPSULE);

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


	//Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AP1Player::DrinkPotion)));
}


void AP1Player::BeginPlay()
{
	Super::BeginPlay();

}

void AP1Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystem();
}

void AP1Player::InitAbilitySystem()
{
	Super::InitAbilitySystem();

	AP1PlayerState* PS = GetPlayerState<AP1PlayerState>();
	if (PS)
	{
		ASC = Cast<UP1AbilitySystemComponent>(PS->GetAbilitySystemComponent());
		ASC->InitAbilityActorInfo(PS, this);

		AttributeSet = PS->GetPlayerAttributeSet();
	}

}


void AP1Player::SetupHUDWidget(UP1HUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->InitHpBar(AttributeSet->GetMaxHp());
		InHUDWidget->UpdateHpBar(AttributeSet->GetHp());

		AttributeSet->OnHpChanged.AddUObject(InHUDWidget, &UP1HUDWidget::UpdateHpBar);
	}
}

USpringArmComponent* AP1Player::GetCameraBoom() const
{
	return CameraBoom;
}


void AP1Player::OnDamaged(float Damage, TObjectPtr<AP1Character> Attacker)
{
	AttributeSet->ApplyDamage(Damage);
	if (AttributeSet->GetHp() == 0)
	{
		OnDead();
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (HitReactionMontage)
	{
		// Convert Yaw -> -180 to 180
		float CharacterYaw = FMath::UnwindDegrees(GetActorRotation().Yaw);
		float AttackerYaw = FMath::UnwindDegrees(Attacker->GetActorRotation().Yaw);
		float YawDifference = AttackerYaw - CharacterYaw;

		// Normalize yaw difference
		if (YawDifference > 180.0f) YawDifference -= 360.0f;
		if (YawDifference < -180.0f) YawDifference += 360.0f;

		FName SectionName;


		if (YawDifference > -45.0f && YawDifference < 45.0f)
		{
			SectionName = TEXT("Back");
		}
		else if (YawDifference > 135.0f || YawDifference < -135.0f)
		{
			SectionName = TEXT("Front");
		}
		else if (YawDifference > 45.0f && YawDifference < 135.0f)
		{
			SectionName = TEXT("Left");
		}
		else
		{
			SectionName = TEXT("Right");
		}

		AnimInstance->Montage_Play(HitReactionMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactionMontage);
	}

}

void AP1Player::OnDead()
{
	if (State == EPlayerState::Dead)
	{
		return;
	}
	State = EPlayerState::Dead;
}

void AP1Player::AttackHitCheck()
{
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = 130.0f;
	const float AttackDamage = AttributeSet->GetBaseDamage();
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_P1ACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		for (FHitResult HitResult : OutHitResults)
		{
			if (AP1Character* HitCharacter = Cast<AP1Character>(HitResult.GetActor()) )
			{
				HitCharacter->OnDamaged(AttributeSet->GetBaseDamage(), this);
			}
		}
	}


#if ENABLE_DRAW_DEBUG
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 0.5f);
#endif
}

void AP1Player::Input_Move(const FInputActionValue& InputValue)
{
	if (GetMyPlayerState() == EPlayerState::Rolling)
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
	if (GetMyPlayerState() == EPlayerState::Rolling)
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
	if (GetMyPlayerState() == EPlayerState::Rolling)
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
	SetPlayerState(EPlayerState::Rolling);
	CurrentCombo = 0;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(ComboAttackMontage))
	{
		AnimInstance->Montage_Stop(0.2f);
	}
	AnimInstance->Montage_Play(RollingMontage, RollingSpeedRate);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AP1Player::RollingEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RollingMontage);
}

void AP1Player::RollingEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	CurrentCombo = 0;
	SetPlayerState(EPlayerState::None);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

EPlayerState AP1Player::GetMyPlayerState()
{
	return State;
}

void AP1Player::SetPlayerState(EPlayerState InState)
{
	this->State = InState;
}


void AP1Player::TakeItem(UP1ItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AP1Player::DrinkPotion(UP1ItemData* InItemData)
{
	AttributeSet->SetHp(FMath::Clamp(AttributeSet->GetHp() + 20, 0, AttributeSet->GetMaxHp()));
}
