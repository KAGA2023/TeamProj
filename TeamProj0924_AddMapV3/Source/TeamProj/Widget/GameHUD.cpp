// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "../Interface/WidgetInterface.h"

void UGameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	SkillSlot->SetVisibility(ESlateVisibility::Hidden);
	if (CntStatusBtn)
	{
		CntStatusBtn->OnClicked.AddDynamic(this, &UGameHUD::OnCntStatusClicked);
		CntStatusBtn->OnHovered.AddDynamic(this, &UGameHUD::OnCntStatusBtnHovered);
		CntStatusBtn->OnUnhovered.AddDynamic(this, &UGameHUD::OnCntStatusBtnUnhovered);
	}
}

void UGameHUD::OnCntStatusBtnHovered()
{
	if (CntGameStage && CntGameStage->GetClass()->ImplementsInterface(UHUDWidgetInterface::StaticClass()))
	{
		IHUDWidgetInterface::Execute_RoundChangeToMorning(CntGameStage);
	}
	CntStatus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}


void UGameHUD::OnCntStatusBtnUnhovered()
{
	if (CntGameStage && CntGameStage->GetClass()->ImplementsInterface(UHUDWidgetInterface::StaticClass()))
	{
		IHUDWidgetInterface::Execute_RoundChangeToNight(CntGameStage);
	}
	CntStatus->SetVisibility(ESlateVisibility::Hidden);
}

void UGameHUD::OnCntStatusClicked()
{
	

}

void UGameHUD::SetSkillSlotVisibility()
{
	UE_LOG(LogTemp, Warning, TEXT("dd"));
	SkillSlot->SetVisibility(ESlateVisibility::Visible);
}
