// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCastle.h"
#include "../MyGamePlayerState.h"
#include "BuildingProductionComponent.h"
#include "../Character/GameCharacterBase.h"
#include "../Status/StatusComponent.h"


AMainCastle::AMainCastle()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMainCastle::ApplyRow(const FBuildingRow& Row)
{
	Super::ApplyRow(Row); //스탯, 리젠 초기화 등등 공통으로 처리할거 ㄱㄱ

	//이 밑에서부턴 메인성 전용 효과쓰~ 리롤권, 재료한도량 뭐 등등

	for (const FBuildingEffect& Eff : Row.Effects)
	{
		switch (Eff.Type)
		{
		case EBuildingEffectType::AddReRoll:
			if (Eff.IOutValue != 0)
			{
				HandleAddReRoll(Eff.IOutValue);
			}
			break;

		case EBuildingEffectType::MatterLimitAdd:
			if (Eff.IOutValue != 0)
			{
				HandleMatterLimitAdd(Eff.IOutValue);
			}
			break;

		default:
			// 메인성 전용이 아닌 효과는 건드리지 않음(공통 처리는 Base에)
			break;
		}
	}

	//특성 UI호출? 대충짜봄
	if (HasAuthority() && bOpenTraitUIOnLevelUp)
	{
		if (bFirstApplyDone) // 이미 한 번 적용된 이후(=레벨업 상황)
		{
			OnTraitSelectionRequested.Broadcast(CurrentLevel);
		}
		bFirstApplyDone = true;
	}

}

AMyGamePlayerState* AMainCastle::GetTPPlayerState() const
{
	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			return Cast<AMyGamePlayerState>(PC->PlayerState);
		}
	}
	return nullptr;
}

void AMainCastle::HandleAddReRoll(int32 Delta)
{
	if (!HasAuthority()) return;

	if (AMyGamePlayerState* PS = GetTPPlayerState())
	{
		//PS->AddRerollCount(Delta); 리롤권 추가 함수 ㅇㅇ
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainBuilding] PlayerState not found for AddReRoll(%d)"), Delta);
	}
}

void AMainCastle::HandleMatterLimitAdd(int32 Delta)
{
	if (!HasAuthority()) return;

	if (AMyGamePlayerState* PS = GetTPPlayerState())
	{
		//PS->IncreaseMatterLimit(Delta); //  한도량 추가 함수 ㅇㅇㅁㄱㅁㄱㅁㄴㅇㄴㅇㅈfucklkkkkkkkkkkkkkk
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainBuilding] PlayerState not found for MatterLimitAdd(%d)"), Delta);
	}
}

