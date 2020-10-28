// Copyright Epic Games, Inc. All Rights Reserved.

#include "TecentLearnProjectGameMode.h"
#include "TecentLearnProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATecentLearnProjectGameMode::ATecentLearnProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
