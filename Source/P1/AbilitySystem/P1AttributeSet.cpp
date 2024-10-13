

#include "AbilitySystem/P1AttributeSet.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "P1GameplayTags.h"
UP1AttributeSet::UP1AttributeSet()
{
	InitMaxHp(100.f);
	InitHp(GetMaxHp());
	InitAttackRange(100.f);
	InitAttackSpeed(1.f);
	InitMovementSpeed(500.f);
	InitAttackRate(30.0f);
	InitDamage(0.0f);
}



void UP1AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

void UP1AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHp(FMath::Clamp(GetHp() - GetDamage(), MinimumHealth, GetMaxHp()));
		OnHpChanged.Broadcast();
		SetDamage(0.0f);
		UE_LOG(LogTemp, Log, TEXT("HP : %lf"), GetHp());
	}

	if (GetHp() <= 0.0f && !bOutOfHp)
	{
		Data.Target.AddLooseGameplayTag(P1GameplayTags::Character_State_IsDead);
		bOutOfHp = true;
	}

}
