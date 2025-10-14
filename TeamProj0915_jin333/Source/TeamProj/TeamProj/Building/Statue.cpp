// Fill out your copyright notice in the Description page of Project Settings.


#include "Statue.h"
#include "ProductionCompo/BPDComponent_Statue.h"
AStatue::AStatue()
{
	ProductionComp = CreateDefaultSubobject<UBPDComponent_Statue>(TEXT("ProductionComp"));
}

void AStatue::PreInit()
{
	SetDataTable(Path);
}

void AStatue::PostInit()
{
}
