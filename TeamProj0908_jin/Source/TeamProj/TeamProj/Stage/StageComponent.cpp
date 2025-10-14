// Fill out your copyright notice in the Description page of Project Settings.


#include "StageComponent.h"
#include "RoundComponent.h"
#include "../MyGamePlayerState.h"

UStageComponent::UStageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	StageTable = nullptr;
	CurRound = 0;
	MaxRound = 0;

}


void UStageComponent::BeginPlay()
{
	Super::BeginPlay();

	Init(EStageID::Stage1);
}

void UStageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



void UStageComponent::Init(EStageID StageId)
{
	
	StageTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Stage/DT_Stage.DT_Stage"));
	if (!StageTable) return;
	UE_LOG(LogTemp, Warning, TEXT("Stage DataTable Load Succeed"));

	FName RowName;
	switch (StageId)
	{
	case EStageID::None:   RowName = NAME_None;       break;
	case EStageID::Stage1: RowName = FName("Stage1"); break;
	case EStageID::Stage2: RowName = FName("Stage2"); break;
	case EStageID::Stage3: RowName = FName("Stage3"); break;
	default:               RowName = NAME_None;       break;
	}

	if (RowName == NAME_None) return;

	StageInfo = StageTable->FindRow<FStage>(RowName, TEXT("Lookup Stage Data"));

	if (StageInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Stage ID = %s, Total Potal Cnt : %d, Total Round Cnt : %d"),
			*StageInfo->StageID.ToString(),
			StageInfo->PortalPos.Num(),
			StageInfo->Round.Num());
		PortalPos = StageInfo->PortalPos;
		Round = StageInfo->Round;
		MaxRound = StageInfo->Round.Num() - 1;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Stage Row not found for %s"), *RowName.ToString());
		return;
	}

	StartStage();
}

void UStageComponent::StartStage()
{
	SelectWeapon();
	SetRound();
}

void UStageComponent::SelectWeapon()
{
	//UI 무기선택 ㅇㅇ
}

void UStageComponent::SetRound()
{
	if (++CurRound <= MaxRound)
		Cast<AMyGamePlayerState>(GetOwner())->RoundCom->InitRound(StageInfo, Round[CurRound]);
}

void UStageComponent::EndStage()
{
	//클리어 유무 값 게임 인스턴스에 저장
}

