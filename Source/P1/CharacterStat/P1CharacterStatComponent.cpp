
#include "CharacterStat/P1CharacterStatComponent.h"


UP1CharacterStatComponent::UP1CharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = 200.0f;
	SetAttackDamage(20.f);
}


void UP1CharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHp(CurrentHp);
}

float UP1CharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UP1CharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.0f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}


void UP1CharacterStatComponent::SetAttackDamage(float Value)
{
	AttackDamage = Value;
}


