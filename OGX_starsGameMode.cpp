// Copyright Epic Games, Inc. All Rights Reserved.

#include "OGX_starsGameMode.h"
#include "OGX_starsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOGX_starsGameMode::AOGX_starsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
