// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "P1Character.generated.h"


USTRUCT(BlueprintType, Category = "GAS")
struct FAbilityWithTag
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UP1GameplayAbility> Ability;
};


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



//GAS
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UP1AbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<FAbilityWithTag> StartAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UP1AttributeSet> AttributeSet;

	FGameplayTagContainer GameplayTagContainer;

public:
	TMap<FGameplayTag, FGameplayAbilitySpec> AbilitySpecMap;
};
