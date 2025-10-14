// Fill out your copyright notice in the Description page of Project Settings.

#include "SellShop.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "../Item/ItemComponent.h"
#include "../MyGamePlayerState.h"
#include "Kismet/GameplayStatics.h"

USellShop::USellShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedItemComponent = nullptr;
}

void USellShop::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind In Button
	if (InButton)
	{
		InButton->OnClicked.AddDynamic(this, &USellShop::OnInButtonClicked);
	}

	// Bind Out Button
	if (OutButton)
	{
		OutButton->OnClicked.AddDynamic(this, &USellShop::OnOutButtonClicked);
	}

	// Bind Sell Button
	if (SellButton)
	{
		SellButton->OnClicked.AddDynamic(this, &USellShop::OnSellButtonClicked);
	}

	// Initialize SellText
	if (SellText)
	{
		SellText->SetText(FText::FromString(TEXT("총 판매가: 0")));
	}
}

void USellShop::OnInButtonClicked()
{
	// TODO: 아이템을 판매 슬롯으로 이동하는 로직 구현
}

void USellShop::OnOutButtonClicked()
{
	// TODO: 아이템을 판매 슬롯에서 제거하는 로직 구현
}

void USellShop::OnSellButtonClicked()
{
	// TODO: 판매 로직 구현
}

void USellShop::SetupSellShop(UItemComponent* ItemComponent)
{
	if (!ItemComponent)
	{
		return;
	}

	CachedItemComponent = ItemComponent;
	
	// TODO: 상점 초기화 로직 구현
}

void USellShop::RefreshSellData()
{
	if (!CachedItemComponent)
	{
		return;
	}

	// TODO: 데이터 새로고침 로직 구현
}