// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace P1GameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Move);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Turn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Rolling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Action_Smash);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Begin);

	//State
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsAttacking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsRolling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsDead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsSmashing);


	//Ability
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Rolling);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Player_Smash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_AttackHitCheck);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Enemy_Attack);

	//Identity
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Identity_Player);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Identity_CommonMonster);

	//AI
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Enemy_AttackWaiting);
}