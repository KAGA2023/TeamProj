// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "Laboratory.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API ALaboratory : public ABuildingBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable Path")
	FString Path = ""; // 데이터테이블 경로 넣기

	virtual void PreInit() override;
	virtual void PostInit() override;
};
