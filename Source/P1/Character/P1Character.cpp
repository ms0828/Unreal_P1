// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P1Character.h"
#include "GameplayTagContainer.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AP1Character::AP1Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AP1Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AP1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AP1Character::OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker)
{
}

void AP1Character::OnDead(TObjectPtr<AP1Character> Attacker)
{
}

void AP1Character::HandleGameplayEvent(FGameplayTag EventTag)
{

}


