// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundComponent.h"
#include "Portal.h"
#include "../MyGamePlayerState.h"  //추가
#include "Kismet/GameplayStatics.h"  //추가
#include "../Perk/PerkComponent.h"  //추가

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
	//�̸� ��Ż ����(���� �κ�����)
	// ��Ż�� ���� ������ �ѱ��.

}

void URoundComponent::SetState(ERoundState state)
{
	switch (state)
	{
	case ERoundState::None:
		break;
	case ERoundState::RoundStart:
	{  //추가
		RoundState = ERoundState::RoundStart;
		OnRoundStart.Broadcast();
		
		// PerkComponent의 라운드 시작 처리  //추가
		APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Pawn)
		{
			if (AMyGamePlayerState* MyPS = Cast<AMyGamePlayerState>(Pawn->GetPlayerState()))
			{
				if (MyPS->PerkComp)
				{
					MyPS->PerkComp->OnRoundStart();
				}
			}
		}
		break;
	}  //추가
	case ERoundState::WarStart:
		RoundState = ERoundState::WarStart;
		OnWarStart.Broadcast();
		// �ǹ� ��ȣ�ۿ� ��Ȱ
		// UI & ����Ʈ���μ��� ����
		// ������ ����-> ��Ż(������)�� ����
		break;
	case ERoundState::WarEnd:
		RoundState = ERoundState::WarEnd;
		OnWarEnd.Broadcast();
		//���������󿡰� ���� �˸�.
		//�÷��̾�ĳ���� status Damage �ʱ�ȭ
		//�ǹ� Destroy
		//���� UI ����
		break;
	case ERoundState::RoundEnd:
	{  //추가
		RoundState = ERoundState::RoundEnd;
		OnRoundEnd.Broadcast();
		
		// PerkComponent의 라운드 종료 처리
		APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);  //추가
		if (Pawn)
		{
			if (AMyGamePlayerState* MyPS = Cast<AMyGamePlayerState>(Pawn->GetPlayerState()))
			{
				if (MyPS->PerkComp)
				{
					MyPS->PerkComp->OnRoundEnd();
				}
			}
		}
		
		//��ȭ ������Ÿ�� Ȱ��ȭ
			// ���� �ѱ��
		//OnRoundFinished.Broadcast();
		// UI & ����Ʈ���μ��� ����
		// StageCom �����ε��� �ٲ㼭 �ѱ�� �Լ�
		break;
	}  //추가
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

	// ���� �޾ƿ���
	// �װŷ� ����
}