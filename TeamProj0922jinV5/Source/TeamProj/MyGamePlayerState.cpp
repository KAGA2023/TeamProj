// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGamePlayerState.h"
#include "Stage/StageComponent.h"
#include "Stage/RoundComponent.h"
#include "Building/BuildingStateComponent.h"
#include "Perk/PerkComponent.h"  //추가

AMyGamePlayerState::AMyGamePlayerState()
{
	StageCom = CreateDefaultSubobject<UStageComponent>(TEXT("StageComponent"));
	RoundCom = CreateDefaultSubobject<URoundComponent>(TEXT("RoundComPonent"));
	BuildingCom = CreateDefaultSubobject<UBuildingStateComponent>(TEXT("BuildingComponent"));
	PerkComp = CreateDefaultSubobject<UPerkComponent>(TEXT("PerkComponent"));  //추가
	Gold = 0;
	Exp = 0;
	Reroll = 0;
}

void AMyGamePlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	// 퍼크 데이터테이블 자동 로드
	if (PerkComp)
	{
		UE_LOG(LogTemp, Log, TEXT("MyGamePlayerState: Auto-loading perk data table"));
		PerkComp->LoadPerksFromDataTable();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MyGamePlayerState: PerkComp is null"));
	}
}
