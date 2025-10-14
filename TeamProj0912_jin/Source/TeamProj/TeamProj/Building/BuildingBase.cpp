// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingBase.h"
#include "../MyGamePlayerState.h"
#include "BuildingProductionComponent.h"
#include "../Character/GameCharacterBase.h"
#include "BuildingStruct.h"
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
	Mesh->SetVisibility(false);

	// 이새끼 안보임..
	Boxcollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollusion"));
	Boxcollision->SetupAttachment(DefaultMesh);
	Boxcollision->SetVisibility(true);
	Boxcollision->SetHiddenInGame(false);
	Boxcollision->SetGenerateOverlapEvents(true);
	Boxcollision->SetCollisionProfileName(TEXT("Trigger")); //오버랩 채널 만들어야할듯.

	StatusComp = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComp"));
	// 건물 기본 : HP / 사망만 사용 ->InitFromBaseStat에 주입

	// 자원 수급용 자식에서 꽂기
	//ProductionComp = CreateDefaultSubobject<UBuildingProductionComponent>(TEXT("ProductionComp"))
}

// Called when the game starts or when spawned
void ABuildingBase::BeginPlay()
{
	Super::BeginPlay();
	UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Building/Mesh/Laboratory/SubBuilding/SM_Laboratory_Workplace.SM_Laboratory_Workplace")));
	if (MeshAsset)
		DefaultMesh->SetStaticMesh(MeshAsset);
	InitBuilding();
}


const FBuildingLevelData* ABuildingBase::GetRow(int32 InLevel) const
{
	UE_LOG(LogTemp, Warning, TEXT("In GetRow"));

	const FName RowName(*FString::FromInt(InLevel));
	UE_LOG(LogTemp, Warning, TEXT("get data by level :  %d"),InLevel);
	const FBuildingLevelData* Row = DataTableAsset->FindRow<FBuildingLevelData>(RowName, TEXT("GetRow"));
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] No Row for Level %d"), *GetName(), InLevel);
	}
	return Row;
}

void ABuildingBase::ApplyRow(const FBuildingLevelData& Row)
{
	//풀피유징ㅇ 
    StatusComp->SetBaseStat(Row.BuildingStat, true);
	
	UpdateVisualForLevel(Row.meshRef);
	overlapoffset = Row.ovelapOffset;
	Boxcollision->SetBoxExtent(Boxcollision->GetScaledBoxExtent() + FVector(overlapoffset,overlapoffset, 0.f));
	
	PostApplyRow();
	//효과 발동. 컴포넌트에 신호주기?
	ProductionComp->InitBuildingEffect(Row);

}

void ABuildingBase::UpdateVisualForLevel(const FString& Ref)
{
	if (Ref.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshRef not set"));
		return;
	}
	
	UStaticMesh* MeshAsset = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),nullptr,*Ref));

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
		if (CurrentLevel > 0) 
		{
			FBoxSphereBounds bound = Mesh->GetStaticMesh()->GetBounds();
			Boxcollision->SetWorldLocation(bound.Origin +GetActorLocation());
			Boxcollision->SetBoxExtent(bound.BoxExtent + FVector(overlapoffset, overlapoffset, 0.f));
		}
	}
}

void ABuildingBase::OnReceiveAnyDamage(float Amount)
{
	StatusComp->TakeDamage(Amount);
}

void ABuildingBase::OnBuildingDied(AActor* DeadOwner) // isDead로 변경 해야함
//지금 껀 아마 다른애가 죽었을때 효과 제어하려고 한듯.
{
	bIsDestroyed = true;

	// 충돌/타겟팅 비활성화
	SetActorEnableCollision(false);

	// 수급 중지
	//if (ProductionComp) ProductionComp->ClearBySource(this);

	// 메시 교체/이펙트/사운드

	// 필요하다면, 게임 시스템에 알림(스테이지 실패 트리거 등)
	// OnBuildingDestroyed.Broadcast(this);

	// 빌딩 스테이트 컴포넌트에 데이터 전송.
	ProductionComp->OnExplosion();

	Destroy();
}

void ABuildingBase::SetDataTable(FString path)
{
	if (!path.IsEmpty())
	{
		UObject* dt = StaticLoadObject(UDataTable::StaticClass(), nullptr, *path);
		if(dt)
			DataTableAsset = Cast<UDataTable>(dt);
		else
			UE_LOG(LogTemp, Warning, TEXT("DataTable Path is Wrong : [%s]"), *path);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("DataTable Path is Empty : [%s]"), *path);
}

void ABuildingBase::OnInteraction()
{
	// 건물 상호작용 -> 플레이어 한테 던지기
	/*플레이어가 오버랩한 건물을 인식하고 플레이어가 신호를 건물에게 주는 방식으로 해결*/
	//플레이어가 오버랩하면 이거 부르면 될듯.
	// UI에 데이터 정보 박아주기.

}

void ABuildingBase::InitBuilding()
{
	UE_LOG(LogTemp, Warning, TEXT("In Initbuilding"));
	PreInit();		// 상속받은 애들에서 구현
	const FBuildingLevelData* Row = GetRow(CurrentLevel);
	if (Row)
	{
		StatusComp->InitFromBaseStat(Row->BuildingStat, EStatusOwnerType::Building, true);
		ApplyRow(*Row);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelData is None!"));
	}
	PostInit();		// 상속받은 애들에서 구현.
}

void ABuildingBase::OpenUI()
{
	//UI 띄우기 이거 병영이랑 상점 같은 경우에는 다르기 때문에 고민해봐야할듯.
}

bool ABuildingBase::LevelUp()
{
	// 레벨업은 위젯에서 호출하는 것으로.
	UE_LOG(LogTemp, Warning, TEXT("In Levelup"));
	if (!bIsBuilt && CurrentLevel==0)
	{
		Mesh->SetVisibility(true);
	}
	const FBuildingLevelData* Next = GetRow(CurrentLevel + 1);
	if (!Next)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] No next level from %d"), *GetName(), CurrentLevel);
		return false;
	}

	CurrentLevel = Next->Level;

	// 베이스 스탯 교체(HP 비율 유지)
	StatusComp->SetBaseStat(Next->BuildingStat,true);

	UE_LOG(LogTemp, Warning, TEXT("In ApplyRow"));
	// 새 레벨 효과 재등록
	ApplyRow(*Next);

	return true;
}

void ABuildingBase::Preview(bool isPressKey)
{
	if (CurrentLevel > 1) return;

	Mesh->SetVisibility(isPressKey);
}

void ABuildingBase::ImportStageData(int32 lv)
{
	CurrentLevel = lv;
	bIsBuilt = lv > 0 ? true : false;
}

void ABuildingBase::SaveStageData(FString actRef, int32 lv, FVector pos, bool isbuilt)
{
}



