// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Skill/SkillStruct.h"
#include "StageStruct.generated.h"

class AGameCharacterBase;
class UStatusComponent;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EClassType ClassType =EClassType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FClassConfig StatID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillConfig> SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillLevel = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowAbstract = "false"))
	//TSoftClassPtr<class AGameCharacterBase> CharClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class UAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector CharScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USkeletalMesh* Chest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<USkeletalMesh*> Parts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMesh* WeaponLeft; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMesh* WeaponRight; 
	
	FCharInfo()
	{
		Parts.SetNum(8);
		CharScale = FVector(1.f, 1.f, 1.f);
	}
	
};

USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FName MobID = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	int32 MonCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	float GenTick = 0.f;
};

USTRUCT(BlueprintType)
struct FSpawnPortal
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FName PortalID = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	TArray<FSpawnEntry> MobInfo;
};



USTRUCT(BlueprintType)
struct FRound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FName RoundID = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	TArray<FSpawnPortal> PortalInfo;
	
};


USTRUCT(BlueprintType)
struct FStage : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FName StageID = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	TArray<FRound> Round;
};



