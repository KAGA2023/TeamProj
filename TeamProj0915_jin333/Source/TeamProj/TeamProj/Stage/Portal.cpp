// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;
	ptData.Empty();
	SpawnPlan.Empty();

}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	if (CharInfoTable && ptData.Num() > 0)
		BuildSpawnPlanFromIDs();
}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::BuildSpawnPlanFromIDs()
{
	SpawnPlan.Reset();

	if (!CharInfoTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Portal] CharInfoTable is null."));
		return;
	}

	for (const FSpawnEntry& Entry : ptData)
	{
		const FCharInfo* Row = CharInfoTable->FindRow<FCharInfo>(Entry.MobID, TEXT("PortalBuild"), true);
		if (!Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Portal] CharInfo row NOT found for MobID: %s"), *Entry.MobID.ToString());
			continue;
		}
		FPortalSpawnInfo Info;
		Info.Entry = Entry;
		Info.CharInfo = *Row;
		SpawnPlan.Add(Info);
	}

}

void APortal::SetDataAndBuild(UDataTable* InCharInfoTable, const TArray<FSpawnEntry>& InEntries)
{
	CharInfoTable = InCharInfoTable;
	ptData = InEntries;
	BuildSpawnPlanFromIDs();
}

