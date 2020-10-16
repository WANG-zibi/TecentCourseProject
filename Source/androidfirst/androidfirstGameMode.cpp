// Copyright Epic Games, Inc. All Rights Reserved.

#include "androidfirstGameMode.h"
#include "androidfirstCharacter.h"
#include "UObject/ConstructorHelpers.h"

AandroidfirstGameMode::AandroidfirstGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
