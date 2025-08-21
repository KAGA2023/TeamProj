// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "../Stage/StageStruct.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "BuildingBase.generated.h"

class UStatusComponent;
//플레이스테이트
class AMyGamePlayerState;
class UBuildingProductionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTraitSelectionRequested, int32, NewLevel);

UENUM(BlueprintType)
enum class EBuildingEffectType : uint8
{
	None,
	AddStat,     //FStatModifier 
	GoldPerTick, //골드수급량 int32
	ExpPerKillRate, //경험치 보정 비율 int32
	DragonEggPerLevel, //드래곤알레벨 int32
	HPRegenPerSec, //틱 회복 flaot
	UnitCapAdd, //병사 젠 수 증가. int32
	RespawnCDDelta, //병수 리젠 감소 float
	TowerAtkAdd, //타워 공격력 float
	TowerAtkSpdAdd, //타워 공격속도 float
	TowerRangeAdd, //타워 공격범위 float
	AddReRoll,     //리롤권 int32
	MatterLimitAdd //자원한도량
	//밑에 건물 효과 추가
};


////////////////////////////////////////베이스/////////////////////////////////////////

USTRUCT(BlueprintType)
struct FBuildingEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) EBuildingEffectType Type = EBuildingEffectType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float   FOutValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32   IOutValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FStatModifier Stat; // Type==AddStat일 때 사용
};

USTRUCT(BlueprintType)
struct FBuildingRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FBaseStat BuildingStat;      // 공통 내구도/방어 등
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 NextLevelCostGold = 0; // 다음 레벨업 비용
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FBuildingEffect> Effects; //다음 레벨업
};















UCLASS(Abstract)
class TEAMPROJ_API ABuildingBase : public AActor
{
	GENERATED_BODY()

	public:
	ABuildingBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DefaultMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStatusComponent* StatusComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBuildingProductionComponent* ProductionComp; // 건물효과담당

	// 에디터에서 지정(Soft 권장). Soft 없으면 UDataTable*로 두고 그대로 사용해도 OK.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSoftObjectPtr<UDataTable> DataTableAsset;

	// 레벨별 메시(옵션)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	TArray<TSoftObjectPtr<UStaticMesh>> LevelMeshes;

	// 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBuilt = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDestroyed = false;

	// AddStat 이전 값 추적(레벨 갱신 시 중복 방지)
	UPROPERTY()
	FStatModifier PrevAddStat;
	UPROPERTY()
	bool bHasPrevAddStat = false;

protected:
	virtual void BeginPlay() override;

	const FBuildingRow* GetRow(int32 InLevel) const;


	UFUNCTION()
	void UpdateVisualForLevel(int32 InLevel);

	UFUNCTION()
	void OnReceiveAnyDamage(float Amount);
	UFUNCTION()
	void OnBuildingDied(AActor* DeadOwner);

public:
	virtual void ApplyRow(const FBuildingRow& Row);

	UFUNCTION(BlueprintCallable)
	bool Build();   // bIsBuilt=true + Level 1 적용

	//디버그용
	UFUNCTION(BlueprintCallable)
	bool DebugBuildAtLevel(int32 TargetLevel);
	

	UFUNCTION(BlueprintCallable)
	bool LevelUp(); // 비용 체크는 외부(경제 시스템)

	UFUNCTION(BlueprintCallable)
	void Preview(bool isPressKey);
};





/////////////////////////////메인/////////////////////////////////

