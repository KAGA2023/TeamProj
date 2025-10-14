// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "ProductionCompo/BPDComponent_Main.h"
AMain::AMain()
{
	ProductionComp = CreateDefaultSubobject<UBPDComponent_Main>(TEXT("ProductionComp"));

	RespawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnPos"));
	RespawnPos->SetupAttachment(DefaultMesh);
	RespawnPos->SetRelativeLocation(FVector(-30.f, 0.f, 0.f));
}

void AMain::PreInit()
{
	SetDataTable(Path);
}

void AMain::PostInit()
{
}



