// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "../Character/GameCharacterBase.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;
	isReady = false;
	SpawnPlan.Empty();

}

void APortal::BeginPlay()
{
	Super::BeginPlay();

}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (isReady)
	{
		ticktime += DeltaTime;
		for (int32 i = 0; i <= SpawnPlan.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Tick] %s"), *this->GetName());
			//SpawnMob(i, ticktime);
		}

	}
	
}

void APortal::BuildSpawnPlanFromIDs(TArray<FSpawnEntry> pptData)
{
	SpawnPlan.Reset();
	UE_LOG(LogTemp, Warning, TEXT("[Portal] Portal BuildSpawnPlan"));

	if (!CharInfoTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Portal] CharInfoTable is null."));
		return;
	}

	for (const FSpawnEntry& Entry : pptData)
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
		GenTickPlan.Add(Info.Entry.GenTick);
		UE_LOG(LogTemp, Warning, TEXT("Portal Mob Info Success"));
		
	}
	isReady = true;

}

void APortal::SpawnMob(int32 MobIndex, float DeltaTime)
{
	if (SpawnPlan[MobIndex].Entry.MobCount <= 0) return;

	UWorld* W = GetWorld();
	if (!W) return;

	GenTickPlan[MobIndex] -= DeltaTime;

	if (GenTickPlan[MobIndex] <= 0.f)
	{
		FActorSpawnParameters SpawnPara;
		SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AEnemyCharacter* EC = W->SpawnActor<AEnemyCharacter>(EnemyBP, GetActorLocation(), FRotator(0.f, 0.f, 0.f), SpawnPara);
		EC->InitCharInfo(SpawnPlan[MobIndex].CharInfo.ClassType, SpawnPlan[MobIndex].CharInfo.StatID, SpawnPlan[MobIndex].CharInfo.SkillID
			, SpawnPlan[MobIndex].CharInfo.SkillLevel, SpawnPlan[MobIndex].CharInfo.Chest, SpawnPlan[MobIndex].CharInfo.Parts, SpawnPlan[MobIndex].CharInfo.WeaponLeft, SpawnPlan[MobIndex].CharInfo.WeaponRight
			, SpawnPlan[MobIndex].CharInfo.AnimClass, SpawnPlan[MobIndex].CharInfo.CharScale);
		EC->Tags.AddUnique(SpawnPlan[MobIndex].CharInfo.StatID.WeaponType);
		GenTickPlan[MobIndex] = SpawnPlan[MobIndex].Entry.GenTick;
		SpawnPlan[MobIndex].Entry.MobCount--;
	}
	


}
