// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/P1InputData.h"

const UInputAction* UP1InputData::FindInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FP1InputAction& Action : InputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Can't find InputAction [%s]"), *InputTag.ToString());

	return nullptr;
}
