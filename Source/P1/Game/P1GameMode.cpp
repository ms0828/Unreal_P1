// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/P1GameMode.h"
#include "Player/P1PlayerState.h"
AP1GameMode::AP1GameMode()
{
	//DefaultPawnClass
	static ConstructorHelpers::FClassFinder<APawn> PlayerClassRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Player.BP_Player_C'"));
	if (PlayerClassRef.Class != nullptr)
	{
		DefaultPawnClass = PlayerClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Input/BP_PlayerController.BP_PlayerController_C'"));
	if (PlayerControllerClassRef.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
		
	PlayerStateClass = AP1PlayerState::StaticClass();
}
