// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P1Character.generated.h"

UCLASS()
class P1_API AP1Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AP1Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnDamaged(int32 Damage, TObjectPtr<AP1Character> Attacker);
	virtual void OnDead(TObjectPtr<AP1Character> Attacker);
	virtual void HandleGameplayEvent(struct FGameplayTag EventTag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> HitReactionMontage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Hp = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FinalDamage = 10;
};
