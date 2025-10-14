// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGamePlayerState.h"
#include "Stage/StageComponent.h"
#include "Stage/RoundComponent.h"

AMyGamePlayerState::AMyGamePlayerState()
{
	StageCom = CreateDefaultSubobject<UStageComponent>(TEXT("StageComponent"));
	RoundCom = CreateDefaultSubobject<URoundComponent>(TEXT("RoundComPonent"));
	Gold = 100;
	Exp = 0;
	Reroll = 0;
}

void AMyGamePlayerState::BeginPlay()
{
	Super::BeginPlay();
}
