

#include "AbilitySystem/P1AttributeSet.h"
#include "AbilitySystem/P1AbilitySystemComponent.h"

UP1AttributeSet::UP1AttributeSet()
{
	InitMaxHp(100.f);
	InitHp(GetMaxHp());
	InitBaseDamage(20.f);
	InitAttackRange(70.f);
	InitAttackSpeed(1.f);
	InitMovementSpeed(500.f);
}

float UP1AttributeSet::ApplyDamage(float InDamage)
{
	const float ActualDamage = FMath::Clamp(InDamage, 0, InDamage);
	const float AfterHp = FMath::Clamp(GetHp() - ActualDamage, 0, GetMaxHp());

	SetHp(AfterHp);
	OnHpChanged.Broadcast(GetHp());

	if (GetHp() <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

