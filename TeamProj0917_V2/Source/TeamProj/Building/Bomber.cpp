// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomber.h"
#include "ProductionCompo/BPDComponent_Bomber.h"

ABomber::ABomber()
{
	ProductionComp = CreateDefaultSubobject<UBPDComponent_Bomber>(TEXT("ProductionComp"));
}

void ABomber::PreInit()
{
	SetDataTable(Path);
}

void ABomber::PostInit()
{
}
