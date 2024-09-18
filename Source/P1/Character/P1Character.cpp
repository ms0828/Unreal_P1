// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1Character.h"
#include "GameplayTagContainer.h"
#include "Components/CapsuleComponent.h"
#include "Physics/P1Collision.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "AbilitySystem/P1EnemyAttributeSet.h"

AP1Character::AP1Character()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_P1CAPSULE);

}

void AP1Character::BeginPlay()
{
	Super::BeginPlay();
	AttributeSet->OnHpChanged.AddDynamic(this, &ThisClass::OnDamaged);
}


void AP1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AP1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AP1Character::OnDamaged()
{
}

void AP1Character::OnDead()
{
}

void AP1Character::HandleGameplayEvent(FGameplayTag EventTag)
{

}
void AP1Character::EnableRagdoll()
{
}

void AP1Character::Disappear()
{
}

UAbilitySystemComponent* AP1Character::GetAbilitySystemComponent() const
{
	return ASC;
}

void AP1Character::InitAbilitySystem()
{

}
