// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGamePlayerState.h"
#include "Stage/StageComponent.h"
#include "Stage/RoundComponent.h"
#include "Character/GameCharacterBase.h"
#include "Building/BuildingStateComponent.h"
#include "Perk/PerkComponent.h"

AMyGamePlayerState::AMyGamePlayerState()
{
	StageCom = CreateDefaultSubobject<UStageComponent>(TEXT("StageComponent"));
	RoundCom = CreateDefaultSubobject<URoundComponent>(TEXT("RoundComPonent"));
	BuildingCom = CreateDefaultSubobject<UBuildingStateComponent>(TEXT("BuildingComponent"));
	PerkComp = CreateDefaultSubobject<UPerkComponent>(TEXT("PerkComponent"));
	Gold = 0;
	Exp = 0;
	Reroll = 0;
}

void AMyGamePlayerState::BeginPlay()
{
	Super::BeginPlay();

	// ��ũ ���������̺� �ڵ� �ε�
	if (PerkComp)
	{
		UE_LOG(LogTemp, Log, TEXT("MyGamePlayerState: Auto-loading perk data table"));
		PerkComp->LoadPerksFromDataTable();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MyGamePlayerState: PerkComp is null"));
	}

	APlayerCharacter* PC = Cast<APlayerCharacter>(GetPawn());
	if (!PC) return;

	// MainCastle이 유효한지 확인 후 접근
	if (BuildingCom && BuildingCom->MainCastle)  //추가2 자체변경함
	{
		FVector loca = BuildingCom->MainCastle->GetActorLocation();
		PC->SetActorLocation(FVector(loca.X - 800.f, loca.Y, loca.Z));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MyGamePlayerState: BuildingCom or MainCastle is null"));
		// 기본 위치로 설정하거나 다른 처리
		PC->SetActorLocation(FVector(0.f, 0.f, 100.f));
	}


}
