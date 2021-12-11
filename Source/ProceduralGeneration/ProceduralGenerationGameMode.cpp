// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProceduralGenerationGameMode.h"
#include "ProceduralGenerationCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ProcGenActor.h"

AProceduralGenerationGameMode::AProceduralGenerationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
