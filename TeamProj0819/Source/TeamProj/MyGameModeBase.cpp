// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyGameController.h"
#include "MyGamePlayerState.h"
#include "MyGameHUD.h"
#include "Character/GameCharacterBase.h"


AMyGameModeBase::AMyGameModeBase()
{
	PlayerStateClass = AMyGamePlayerState::StaticClass();
	PlayerControllerClass = AMyGameController::StaticClass();
	HUDClass = AMyGameHUD::StaticClass();
	
	static ConstructorHelpers::FClassFinder<APlayerCharacter> BP_Player(TEXT("/Game/ThirdPerson/Blueprints/BP_Player.BP_Player_C"));
	if (BP_Player.Succeeded())
	{
		DefaultPawnClass = BP_Player.Class;
	}

	//DefaultPawnClass = APlayerCharacter::StaticClass();
}
