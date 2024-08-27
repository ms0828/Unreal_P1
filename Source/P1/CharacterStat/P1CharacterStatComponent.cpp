
#include "CharacterStat/P1CharacterStatComponent.h"
#include "Data/P1GameSingleton.h"

UP1CharacterStatComponent::UP1CharacterStatComponent()
{
	CurrentLevel = 1;

	bWantsInitializeComponent = true;
}

void UP1CharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
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
	CurrentHp = FMath::Clamp(NewHp, 0.0f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}

void UP1CharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UP1GameSingleton::Get().PlayerMaxLevel);
	SetBaseStat(UP1GameSingleton::Get().GetPlayerStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}




