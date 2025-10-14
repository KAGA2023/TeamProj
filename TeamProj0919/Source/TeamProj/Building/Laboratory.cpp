// Fill out your copyright notice in the Description page of Project Settings.


#include "Laboratory.h"
#include "ProductionCompo/BPDComponent_Laboratory.h"

ALaboratory::ALaboratory()
{
	ProductionComp = CreateDefaultSubobject<UBPDComponent_Laboratory>(TEXT("ProductionComp"));
}

void ALaboratory::PreInit()
{
	SetDataTable(Path);
}

void ALaboratory::PostInit()
{
}
