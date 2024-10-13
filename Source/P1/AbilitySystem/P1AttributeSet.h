
#pragma once


#include "AttributeSet.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "P1AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)			\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)				\
	

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpChangedDelegate);


UCLASS()
class P1_API UP1AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UP1AttributeSet();


public:
	

public:
	ATTRIBUTE_ACCESSORS(ThisClass, Hp);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHp);
	ATTRIBUTE_ACCESSORS(ThisClass, AttackRange);
	ATTRIBUTE_ACCESSORS(ThisClass, AttackSpeed);
	ATTRIBUTE_ACCESSORS(ThisClass, MovementSpeed);
	ATTRIBUTE_ACCESSORS(ThisClass, AttackRate);
	ATTRIBUTE_ACCESSORS(ThisClass, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	mutable FOnHpZeroDelegate OnHpZero;
	mutable FOnHpChangedDelegate OnHpChanged;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Hp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData AttackSpeed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MovementSpeed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Damage;
	
	
	bool bOutOfHp = false;
	

	friend class UP1GE_AttackDamage;
};
