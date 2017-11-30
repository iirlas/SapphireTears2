// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Sapphire2GameMode.h"
#include "Sapphire2HUD.h"
#include "Sapphire2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayerController.h"

ASapphire2GameMode::ASapphire2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PlayerControllerClass = AMyPlayerController::StaticClass();

	// use our custom HUD class
	HUDClass = ASapphire2HUD::StaticClass();
}
