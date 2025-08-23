// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "../Character/GameCharacterBase.h"
#include "../Status/StatusComponent.h"


void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_CCType->SetVisibility(ESlateVisibility::Hidden);
	m_StatusEffectIcon->SetVisibility(ESlateVisibility::Hidden);



}

void UHealthBarWidget::Init(AGameCharacterBase* pClass)
{
	if (!pClass) return;

	if (APlayerCharacter* P = Cast<APlayerCharacter>(pClass))
		Class = Cast<APlayerCharacter>(pClass);
	else
		return;
	if (AAllyCharacter * P = Cast<AAllyCharacter>(pClass))
		Class = Cast<AAllyCharacter>(pClass);
	else
		return;
	if (AEnemyCharacter * P = Cast<AEnemyCharacter>(pClass))
		Class = Cast<AEnemyCharacter>(pClass);
	else
		return;

	
	
}
