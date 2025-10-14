// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundStruct.h"
#include "StageStruct.h"
#include "Portal.generated.h"

USTRUCT(BlueprintType)
struct FPortalSpawnInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FSpawnEntry Entry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FCharInfo CharInfo;
};

UCLASS()
class TEAMPROJ_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal();

	UPROPERTY(BlueprintReadOnly)
	TArray<FSpawnEntry> ptData;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	UDataTable* CharInfoTable = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Spawn")
	TArray<FPortalSpawnInfo> SpawnPlan;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void BuildSpawnPlanFromIDs();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SetDataAndBuild(UDataTable* InCharInfoTable, const TArray<FSpawnEntry>& InEntries);


};
