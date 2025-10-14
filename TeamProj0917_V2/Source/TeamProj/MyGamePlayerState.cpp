// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGamePlayerState.h"
#include "Stage/StageComponent.h"
#include "Stage/RoundComponent.h"
#include "Building/BuildingStateComponent.h"

AMyGamePlayerState::AMyGamePlayerState()
{
	StageCom = CreateDefaultSubobject<UStageComponent>(TEXT("StageComponent"));
	RoundCom = CreateDefaultSubobject<URoundComponent>(TEXT("RoundComPonent"));
	BuildingCom = CreateDefaultSubobject<UBuildingStateComponent>(TEXT("BuildingComponent"));
	Gold = 0;
	Exp = 0;
	Reroll = 0;
}

void AMyGamePlayerState::BeginPlay()
{
	Super::BeginPlay();
}
