// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../AICon/GameUnitBase.h"
#include "../Status/StatusStruct.h"
#include "BuildingStruct.generated.h"

/**
 * 
 */
UENUM()
enum class EBuildingEffectType : uint8
{
	None,
	Main_AddStat,     //FStatModifier 
	Inn_Gold, //골드수급량 int32
	Statue_Exp, //경험치 보정 비율 int32
	DragonEggPerLevel, //드래곤알레벨 int32
	Laboratory_HPRegenPerSec, //틱 회복 flaot
	Barrack_UnitCapAdd, //병사 젠 수 증가. int32
	Barrack_RespawnCDDelta, //병수 리젠 감소 float
	Tower_AtkAdd, //타워 공격력 float
	Tower_AtkSpdAdd, //타워 공격속도 float
	Tower_RangeAdd, //타워 공격범위 float
	Main_AddReRoll,     //리롤권 int32
	Bomber_ExplosionDmg, //자원한도량
	Bomber_ExplosionRange //자원한도량
};

USTRUCT(BlueprintType)
struct FBuildingEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) EBuildingEffectType Type = EBuildingEffectType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32   intValue;				// 아웃풋 밸류들
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float   floatValue_1;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float   floatValue_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float   floatValue_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool isGlobal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<ETeam> targetTeams;			// 효과 적용 대상 팀들
};

USTRUCT(BlueprintType)
struct FBuildingLevelData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString meshRef;					// 메쉬 레퍼런스
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ovelapOffset;					// 오버랩 가능 오프셋 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FBaseStat BuildingStat;		

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 UpgradeCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FBuildingEffect> buildingEffects;
};
