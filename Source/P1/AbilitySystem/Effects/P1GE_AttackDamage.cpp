
#include "AbilitySystem/Effects/P1GE_AttackDamage.h"
#include "AbilitySystem/P1AttributeSet.h"
UP1GE_AttackDamage::UP1GE_AttackDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UP1AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UP1AttributeSet, Hp)));
	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat DamageAmount(-30.f);
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmount);

	HealthModifier.ModifierMagnitude = ModMagnitude;
	Modifiers.Add(HealthModifier);

}
