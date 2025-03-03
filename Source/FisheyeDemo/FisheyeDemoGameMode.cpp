// Copyright Epic Games, Inc. All Rights Reserved.

#include "FisheyeDemoGameMode.h"
#include "FisheyeDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFisheyeDemoGameMode::AFisheyeDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
