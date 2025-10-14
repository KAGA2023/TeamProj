// Fill out your copyright notice in the Description page of Project Settings.


#include "BPDComponent_Main.h"

UBPDComponent_Main::UBPDComponent_Main()
{
	OwnEffect = { EBuildingEffectType::Main_AddReRoll,EBuildingEffectType::Main_AddStat };
}

void UBPDComponent_Main::InitChild()
{
	for (FBuildingEffect ef : Effects) 
	{
		OnActivate(ef);
	}
}

void UBPDComponent_Main::ActivateEffect(const FBuildingEffect& Effect)
{

}
