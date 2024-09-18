// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "P1Character.generated.h"

UCLASS()
class P1_API AP1Character : public ACharacter, public IAbilitySystemInterface
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

	UFUNCTION()
	virtual void OnDamaged();

	virtual void OnDead();
	virtual void HandleGameplayEvent(struct FGameplayTag EventTag);
	virtual void EnableRagdoll();
	virtual void Disappear();

//GAS
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitAbilitySystem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> HitReactionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadAnimMontage;

	FTimerHandle DeadAnimationTimerHandle;

protected:
	// ----> 두번째 스텟 구현 (스텟 컴포넌트를 만들어 따로 관리)
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP1CharacterStatComponent> Stat;*/

public:
	// ----> 첫번째 스텟 구현 (하드 코딩) (클래스에 멤버로 직접 선언하니 관리가 힘듬)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 Hp = 100;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 MaxHp = 100;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 FinalDamage = 20;



//GAS
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UP1AbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UP1AttributeSet> AttributeSet;

	FGameplayTagContainer GameplayTagContainer;
};
