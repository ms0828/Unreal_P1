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
#include "UI/P1HUDWidget.h"
#include "Item/P1ItemData.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "AbilitySystem/P1PlayerAttributeSet.h"
#include "AbilitySystem/Abilities/P1GameplayAbility.h"
#include "Player/P1PlayerState.h"
#include "GameplayTagContainer.h"
#include "P1GameplayTags.h"

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

	FGameplayTag PlayerTag = P1GameplayTags::Character_Identity_Player;
	GameplayTagContainer.AddTag(PlayerTag);

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

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility.Ability);
			AbilitySpecMap.Add(StartAbility.Tag, StartSpec);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		AP1PlayerController* P1Controller = GetController<AP1PlayerController>();
		if (P1Controller && ASC)
		{
			P1Controller->SetupGASInputComponent(ASC);
		}

	}

}


void AP1Player::SetupHUDWidget(UP1HUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->SetAbilitySystemComponent(ASC);
		InHUDWidget->InitHpBar(AttributeSet->GetMaxHp());
		InHUDWidget->UpdateHpBar(AttributeSet->GetHp());	
	}
}

USpringArmComponent* AP1Player::GetCameraBoom() const
{
	return CameraBoom;
}


void AP1Player::OnDamaged()
{
	
	if (AttributeSet->GetHp() == 0)
	{
		OnDead();
	}
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Stop(1.0f);
	if (HitReactionMontage)
	{
		AnimInstance->Montage_Play(HitReactionMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(TEXT("Front"), HitReactionMontage);
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

UP1ComboAttackData* AP1Player::GetComboAttackData()
{
	return ComboAttackData;
}

UAnimMontage* AP1Player::GetComboAttackMontage()
{
	return ComboAttackMontage;
}

UAnimMontage* AP1Player::GetRollingMontage()
{
	return RollingMontage;
}

UAnimMontage* AP1Player::GetSmashMontage()
{
	return SmashMontage;
}

uint8 AP1Player::GetCurrentCombo()
{
	return CurrentCombo;
}

void AP1Player::SetCurrentCombo(uint8 ComboIndex)
{
	CurrentCombo = ComboIndex;
}

void AP1Player::Input_Move(const FInputActionValue& InputValue)
{

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



EPlayerState AP1Player::GetMyPlayerState()
{
	return State;
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



void AP1Player::GasInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AP1Player::GasInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}

	}
}
