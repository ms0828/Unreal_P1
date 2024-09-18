// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/P1GameSingleton.h"

UP1GameSingleton::UP1GameSingleton()
{

}

UP1GameSingleton& UP1GameSingleton::Get()
{
	UP1GameSingleton* Singleton = CastChecked<UP1GameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	return *NewObject<UP1GameSingleton>();
}
