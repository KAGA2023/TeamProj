// Fill out your copyright notice in the Description page of Project Settings.


#include "Inn.h"
#include "ProductionCompo/BPDComponent_Inn.h"

AInn::AInn()
{
	ProductionComp = CreateDefaultSubobject<UBPDComponent_Inn>(TEXT("ProductionComp"));
}

void AInn::PreInit()
{
	SetDataTable(Path);
}

void AInn::PostInit()
{
}
