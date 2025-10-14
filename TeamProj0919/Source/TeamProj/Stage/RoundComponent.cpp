// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundComponent.h"
#include "Portal.h"

URoundComponent::URoundComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	TotalMobCount = 0;

}

void URoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



void URoundComponent::InitRound(FStage* stageInfo, FRound& roundInfo)
{
	SetPortalPos(stageInfo->PortalPos);
	TotalMobCount = 0;

	for (FSpawnPortal Pt : roundInfo.PortalInfo)
	{
		FString L, R;
		if (Pt.PortalID.ToString().Split(TEXT("_"), &L, &R))
		{
			if (R.IsNumeric())
			{
				int id = FCString::Atoi(*R);
				FActorSpawnParameters SpawnPara;
				SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				UE_LOG(LogTemp, Warning, TEXT("World pointer: %s"), GetWorld() ? TEXT("Valid") : TEXT("Null"));
				APortal* SpawnPt = GetWorld()->SpawnActor<APortal>(PortalBP, PortalPos[id - 1], FRotator(30.f, 0.f, 0.f), SpawnPara);
				if (!SpawnPt)
					return;
				UE_LOG(LogTemp, Warning, TEXT("Succesed Spawn Portal"));
				SpawnPt->BuildSpawnPlanFromIDs(Pt.MobInfo);
			}
		}
		
		for (FSpawnEntry mob : Pt.MobInfo)
			TotalMobCount += mob.MobCount;
	}
	UE_LOG(LogTemp, Warning, TEXT("Current Rind Total mob Cnt = %d"), TotalMobCount);
	//SetState(ERoundState::RoundStart);
	//미리 포탈 생성(비긴시 인비지블)
	// 포탈에 몬스터 정보값 넘기기.

}

void URoundComponent::SetState(ERoundState state)
{
	switch (state)
	{
	case ERoundState::None:
		break;
	case ERoundState::RoundStart:
		RoundState = ERoundState::RoundStart;
		OnRoundStart.Broadcast();
		break;
	case ERoundState::WarStart:
		RoundState = ERoundState::WarStart;
		OnWarStart.Broadcast();
		// 건물 상호작용 비활
		// UI & 포스트프로세스 변경
		// 몹스폰 시작-> 포탈(비지블)에 연결
		break;
	case ERoundState::WarEnd:
		RoundState = ERoundState::WarEnd;
		OnWarEnd.Broadcast();
		//여관조각상에게 종료 알림.
		//플레이어캐릭터 status Damage 초기화
		//건물 Destroy
		//정산 UI 띄우기
		break;
	case ERoundState::RoundEnd:
		RoundState = ERoundState::RoundEnd;
		OnRoundEnd.Broadcast();
		//재화 프로젝타일 활성화
			// 라운드 넘기기
		//OnRoundFinished.Broadcast();
		// UI & 포스트프로세스 변경
		// StageCom 라운드인덱스 바꿔서 넘기는 함수
		break;
	default:
		break;
	}
}

void URoundComponent::InitRoundStart()
{
	if (RoundState != ERoundState::RoundStart)
		SetState(ERoundState::RoundStart);
}

void URoundComponent::InitRoundEnd()
{
	if (RoundState != ERoundState::RoundEnd)
		SetState(ERoundState::RoundEnd);
}

void URoundComponent::InitWarStart()
{
	if (RoundState != ERoundState::WarStart)
		SetState(ERoundState::WarStart);
}

void URoundComponent::InitWarEnd()
{
	if (RoundState != ERoundState::WarEnd)
		SetState(ERoundState::WarEnd);
}



void URoundComponent::SetPortalPos(TArray<FVector> ptps)
{
	for (FVector ps : ptps)
	{
		PortalPos.Add(ps);
	}
}

void URoundComponent::CheckMonsterCount()
{
	TotalMobCount--;
	if (TotalMobCount == 0)
		InitWarEnd();
}




void URoundComponent::BeginPlay()
{
	Super::BeginPlay();

	// 라운드 받아오기
	// 그거로 인잇
}