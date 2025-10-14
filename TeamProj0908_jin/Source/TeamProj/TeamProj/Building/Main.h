// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "Main.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API AMain : public ABuildingBase
{
	GENERATED_BODY()
	
public:
	AMain();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Respawn")
	USceneComponent* RespawnPos;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable Path")
	FString Path = "/Game/Data/Building/DT_Main2.DT_Main2"; // 데이터테이블 경로 넣기

	virtual void PreInit() override;
	virtual void PostInit() override;


public:
	
};
