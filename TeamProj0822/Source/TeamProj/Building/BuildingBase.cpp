// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"
#include "../MyGamePlayerState.h"
#include "BuildingProductionComponent.h"
#include "../Character/GameCharacterBase.h"
#include "../Status/StatusComponent.h"

// Sets default values
ABuildingBase::ABuildingBase()
{
 	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);
	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultMesh"));
	DefaultMesh->SetupAttachment(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(DefaultMesh);


	StatusComp = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComp"));
	// 건물 기본 : HP / 사망만 사용 ->InitFromBaseStat에 주입

	// 자원 수급용
	ProductionComp = CreateDefaultSubobject<UBuildingProductionComponent>(TEXT("ProductionComp"));

}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();

	// 이미 에디터에서 ‘지어진 상태’로 배치할 수도 있으니 처리
	if (bIsBuilt && CurrentLevel > 0)
	{
		const FBuildingRow* Row = GetRow(CurrentLevel);
		if (Row)
		{
			// 이미 지어진 상태라면 레벨 Row를 적용(HP 풀로 시작ㄳㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱㄱ)
			StatusComp->InitFromBaseStat(Row->BuildingStat, EStatusOwnerType::Building, true);
			ApplyRow(*Row);
			UpdateVisualForLevel(CurrentLevel);
		}
	}

	// 파괴 이벤트 바인딩
	StatusComp->OnDied.AddDynamic(this, &ABuildingBase::OnBuildingDied);
}


const FBuildingRow* ABuildingBase::GetRow(int32 InLevel) const
{
	if (InLevel <= 0) return nullptr;

	UDataTable* DT = nullptr;
	if (DataTableAsset.IsValid())
	{
		DT = DataTableAsset.Get();
	}
	else if (DataTableAsset.ToSoftObjectPath().IsValid())
	{
		DT = DataTableAsset.LoadSynchronous();
	}

	if (!DT)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] DataTable not set"), *GetName());
		return nullptr;
	}

	const FName RowName(*FString::FromInt(InLevel));
	const FBuildingRow* Row = DT->FindRow<FBuildingRow>(RowName, TEXT("GetRow"));
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] No Row for Level %d"), *GetName(), InLevel);
	}
	return Row;
}

void ABuildingBase::ApplyRow(const FBuildingRow& Row)
{
	//풀피유징ㅇ 
    StatusComp->SetBaseStat(Row.BuildingStat, true);
	if (bHasPrevAddStat)
	{
		StatusComp->RemoveModifier(PrevAddStat);
		bHasPrevAddStat = false;
	}

	
	// 2) 이전 효과 정리
	if (bHasPrevAddStat)
	{
		StatusComp->RemoveModifier(PrevAddStat);
		bHasPrevAddStat = false;
	}
	/*
	if (ProductionComp)
	{
		// 소스=이 빌딩으로 등록된 모든 생산 항목 제거(중복 방지)
		ProductionComp->ClearBySource(this);
	}
	*/
	StatusComp->SetHPRegenPerSec(0.f); // 리젠류 재설정
	

	//건물고유효과 관리 -> 건물컴포넌트에서 
	for (const FBuildingEffect& Eff : Row.Effects)
	{
		switch (Eff.Type)
		{
		case EBuildingEffectType::AddStat:
			StatusComp->AddModifier(Eff.Stat);
			PrevAddStat = Eff.Stat;
			bHasPrevAddStat = true;
			break;
		default:
			break;
		}
	}

	/*
		case EBuildingEffectType::GoldPerTick:
			if (ProductionComp && Eff.FValue != 0.f)
			{
				ProductionComp->RegisterIncome(EResourceType::Gold,
					Eff.FValue,
					this,
					TEXT("GoldPerTick"));
			}
			break;

		case EBuildingEffectType::HPRegenPerSec:
			StatusComp->SetHPRegenPerSec(Eff.FValue);
			break;

		case EBuildingEffectType::AddReRoll:
			// PlayerState 등으로 라우팅(느슨 결합 권장: 델리게이트/인터페이스)
			// OnRequestAddReroll.Broadcast(Eff.IValue);
			break;

		case EBuildingEffectType::MatterLimitAdd:
			// OnRequestMatterLimitAdd.Broadcast(Eff.IValue);
			break;

		case EBuildingEffectType::TowerAtkAdd:
		case EBuildingEffectType::TowerAtkSpdAdd:
		case EBuildingEffectType::TowerRangeAdd:
			// if (ITowerReceiver* Rcv = Cast<ITowerReceiver>(this)) { Rcv->Execute_ApplyTowerEffect(this, Eff); }
			break;

		case EBuildingEffectType::UnitCapAdd:
		case EBuildingEffectType::RespawnCDDelta:
			// if (IBarrackReceiver* Rcv = Cast<IBarrackReceiver>(this)) { Rcv->Execute_ApplyBarrackEffect(this, Eff); }
			break;

		case EBuildingEffectType::ExpPerKillRate:
		case EBuildingEffectType::DragonEggPerLevel:
			// TODO: 당신 게임 규칙에 맞게 라우팅
			break;

		default:
			break;
		}
	}
	*/

	// 4) 비주얼
	UpdateVisualForLevel(Row.Level);
}

void ABuildingBase::UpdateVisualForLevel(int32 InLevel)
{
	const int32 Index = FMath::Max(0, InLevel - 1);
	if (LevelMeshes.IsValidIndex(Index))
	{
		UStaticMesh* MeshAsset = nullptr;
		if (LevelMeshes[Index].IsValid())
		{
			MeshAsset = LevelMeshes[Index].Get();
		}
		else if (LevelMeshes[Index].ToSoftObjectPath().IsValid())
		{
			MeshAsset = LevelMeshes[Index].LoadSynchronous();
		}

		if (MeshAsset)
		{
			Mesh->SetStaticMesh(MeshAsset);
		}
	}

	// 파괴 상태이후 처리 뿌서진거 하던지 뭐하던지
	if (bIsDestroyed)
	{
		// 폐허 메시/파티클/머티 적용
	}
}

void ABuildingBase::OnReceiveAnyDamage(float Amount)
{
	StatusComp->TakeDamage(Amount);
}

void ABuildingBase::OnBuildingDied(AActor* DeadOwner)
{
	bIsDestroyed = true;

	// 충돌/타겟팅 비활성화
	SetActorEnableCollision(false);

	// 수급 중지
	//if (ProductionComp) ProductionComp->ClearBySource(this);

	// 메시 교체/이펙트/사운드
	UpdateVisualForLevel(CurrentLevel);

	// 필요하다면, 게임 시스템에 알림(스테이지 실패 트리거 등)
	// OnBuildingDestroyed.Broadcast(this);
}

bool ABuildingBase::Build()
{
	return DebugBuildAtLevel(1);  //디버깅 테스트
	/*
	if (bIsBuilt || bIsDestroyed)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Already built or destroyed"), *GetName());
		return false;
	}

	const FBuildingRow* Row = GetRow(1);
	if (!Row) return false;

	bIsBuilt = true;
	CurrentLevel = Row->Level;

	// HP 풀로 세팅
	StatusComp->InitFromBaseStat(Row->BuildingStat, EStatusOwnerType::Building, true);

	// 효과 적용(골드틱/리젠/추가 스탯 등)
	ApplyRow(*Row);

	// 비주얼
	UpdateVisualForLevel(CurrentLevel);
	return true;
	*/
}


bool ABuildingBase::DebugBuildAtLevel(int32 TargetLevel)
{
	if (bIsBuilt || bIsDestroyed) return false;

	const FBuildingRow* Row = GetRow(TargetLevel);
	if (!Row) return false;

	bIsBuilt = true;
	CurrentLevel = Row->Level;

	// HP 풀로 시작
	StatusComp->InitFromBaseStat(Row->BuildingStat, EStatusOwnerType::Building, true);

	ApplyRow(*Row);
	UpdateVisualForLevel(CurrentLevel);
	return true;
}



bool ABuildingBase::LevelUp()
{
	if (!bIsBuilt || bIsDestroyed) return false;
	Mesh->SetVisibility(true);
	const FBuildingRow* Next = GetRow(CurrentLevel + 1);
	if (!Next)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] No next level from %d"), *GetName(), CurrentLevel);
		return false;
	}

	// 외부 경제 시스템에서 비용 검증 완료 가정

	CurrentLevel = Next->Level;

	// 베이스 스탯 교체(HP 비율 유지)
	StatusComp->SetBaseStat(Next->BuildingStat,true);

	// 새 레벨 효과 재등록
	ApplyRow(*Next);

	// 비주얼
	UpdateVisualForLevel(CurrentLevel);
	return true;
}

void ABuildingBase::Preview(bool isPressKey)
{
	if (CurrentLevel > 1) return;

	Mesh->SetVisibility(isPressKey);
}

