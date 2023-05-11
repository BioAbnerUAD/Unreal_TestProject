// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_Test_ProjectGameMode.h"
#include "Unreal_Test_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnreal_Test_ProjectGameMode::AUnreal_Test_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
