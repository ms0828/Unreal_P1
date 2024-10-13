
#include "P1GameplayTags.h"

namespace P1GameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Move, "Input.Action.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Turn, "Input.Action.Turn");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Rolling, "Input.Action.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Attack, "Input.Action.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Input_Action_Smash, "Input.Action.Smash");

	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_Begin, "Event.Montage.Begin");

	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsAttacking, "Character.State.IsAttacking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsRolling, "Character.State.IsRolling");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsDead, "Character.State.IsDead");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsSmashing, "Character.State.IsSmashing");


	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Attack, "Ability.Player.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Rolling, "Ability.Player.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Player_Smash, "Ability.Player.Smash");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_AttackHitCheck, "Ability.Action.AttackHitCheck");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Enemy_Attack, "Ability.Enemy.Attack");


	UE_DEFINE_GAMEPLAY_TAG(Character_Identity_Player, "Character.Identity.Player");
	UE_DEFINE_GAMEPLAY_TAG(Character_Identity_CommonMonster, "Character.Identity.CommonMonster");

	//AI
	UE_DEFINE_GAMEPLAY_TAG(AI_Enemy_AttackWaiting, "AI.Enemy.AttackWaiting");


	//SetByCaller Data
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage_Normal, "Data.Damage.Normal");
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage_Critical , "Data.Damage.Critical");
}