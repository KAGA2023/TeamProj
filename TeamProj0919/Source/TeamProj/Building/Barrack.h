// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "Barrack.generated.h"

UENUM()
enum class EIdentity : uint8
{
	L,
	R
};

UCLASS()
class TEAMPROJ_API ABarrack : public ABuildingBase
{
	GENERATED_BODY()

public:
	ABarrack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> SpawnPosList
	{
		FVector(-150.f,-225.f,0.f),FVector(-150.f,-75.f,0.f),
		FVector(-150.f,75.f,0.f),FVector(-150.f,225.f,0.f),
		FVector(-300.f,-225.f,0.f),FVector(-300.f,-75.f,0.f),
		FVector(-300.f,75.f,0.f),FVector(-300.f,225.f,0.f)
	};

	TArray<FVector> CurSpawnPos;

private:
	void SetSpawnposList();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable Path")
	FString Path = "/Game/Data/Building/DT_Barrack.DT_Barrack"; // 데이터테이블 경로 넣기

	FString chPath = "/Game/Data/Class/DT_CharInfo_Barrack.DT_CharInfo_Barrack";

	virtual void BeginPlay() override;

	virtual void PostApplyRow() override;

	virtual void PreInit() override;


	void InitIdentity();

	void SelectIdentity(EIdentity EIden);


	virtual void SetIdenL() { curIden = EIdentity::L;  };
	virtual void SetIdenR() { curIden = EIdentity::R; };

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	EIdentity curIden;
	
	// UI에서 호출하면됨.
	UFUNCTION(BlueprintCallable)
	void InitSelectL();
	UFUNCTION(BlueprintCallable)
	void InitSelectR();
};
