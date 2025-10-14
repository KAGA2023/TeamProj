// Fill out your copyright notice in the Description page of Project Settings.

#include "SmallShopWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

USmallShopWidget::USmallShopWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Initialize slot arrays with default values
	SlotCounts.Init(0, 5);
	SlotPrices.Init(0, 5);
	SlotStacks.Init(0, 5);
}

void USmallShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind Purchase Button
	if (PurchaseButton)
	{
		PurchaseButton->OnClicked.AddDynamic(this, &USmallShopWidget::OnPurchaseButtonClicked);
	}

	// Bind Slot 1 Buttons
	if (Slot1Down)
	{
		Slot1Down->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot1DownClicked);
	}
	if (Slot1Up)
	{
		Slot1Up->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot1UpClicked);
	}

	// Bind Slot 2 Buttons
	if (Slot2Down)
	{
		Slot2Down->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot2DownClicked);
	}
	if (Slot2Up)
	{
		Slot2Up->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot2UpClicked);
	}

	// Bind Slot 3 Buttons
	if (Slot3Down)
	{
		Slot3Down->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot3DownClicked);
	}
	if (Slot3Up)
	{
		Slot3Up->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot3UpClicked);
	}

	// Bind Slot 4 Buttons
	if (Slot4Down)
	{
		Slot4Down->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot4DownClicked);
	}
	if (Slot4Up)
	{
		Slot4Up->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot4UpClicked);
	}

	// Bind Slot 5 Buttons
	if (Slot5Down)
	{
		Slot5Down->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot5DownClicked);
	}
	if (Slot5Up)
	{
		Slot5Up->OnClicked.AddDynamic(this, &USmallShopWidget::OnSlot5UpClicked);
	}

	// Initialize default values
	UpdatePurchaseText(TEXT("구매하기"));
	
	// Set default slot values
	for (int32 i = 0; i < 5; i++)
	{
		UpdateSlotCount(i + 1, 0);
		UpdateSlotPrice(i + 1, 100 * (i + 1)); // Default prices: 100, 200, 300, 400, 500
		UpdateSlotStack(i + 1, 1);
	}
	
	// Initialize total price
	UpdateTotalPrice();
}

void USmallShopWidget::OnPurchaseButtonClicked()
{
	// TODO: Implement purchase logic
	UE_LOG(LogTemp, Warning, TEXT("Purchase Button Clicked!"));
}

void USmallShopWidget::OnSlot1DownClicked()
{
	if (SlotCounts[0] > 0)
	{
		SlotCounts[0]--;
		UpdateSlotCount(1, SlotCounts[0]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot1UpClicked()
{
	if (SlotCounts[0] < SlotStacks[0])
	{
		SlotCounts[0]++;
		UpdateSlotCount(1, SlotCounts[0]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot2DownClicked()
{
	if (SlotCounts[1] > 0)
	{
		SlotCounts[1]--;
		UpdateSlotCount(2, SlotCounts[1]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot2UpClicked()
{
	if (SlotCounts[1] < SlotStacks[1])
	{
		SlotCounts[1]++;
		UpdateSlotCount(2, SlotCounts[1]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot3DownClicked()
{
	if (SlotCounts[2] > 0)
	{
		SlotCounts[2]--;
		UpdateSlotCount(3, SlotCounts[2]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot3UpClicked()
{
	if (SlotCounts[2] < SlotStacks[2])
	{
		SlotCounts[2]++;
		UpdateSlotCount(3, SlotCounts[2]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot4DownClicked()
{
	if (SlotCounts[3] > 0)
	{
		SlotCounts[3]--;
		UpdateSlotCount(4, SlotCounts[3]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot4UpClicked()
{
	if (SlotCounts[3] < SlotStacks[3])
	{
		SlotCounts[3]++;
		UpdateSlotCount(4, SlotCounts[3]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot5DownClicked()
{
	if (SlotCounts[4] > 0)
	{
		SlotCounts[4]--;
		UpdateSlotCount(5, SlotCounts[4]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::OnSlot5UpClicked()
{
	if (SlotCounts[4] < SlotStacks[4])
	{
		SlotCounts[4]++;
		UpdateSlotCount(5, SlotCounts[4]);
		UpdateTotalPrice();
	}
}

void USmallShopWidget::UpdateSlotCount(int32 SlotIndex, int32 NewCount)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	// Stack 범위 내에서만 Count 설정 (0 ~ Stack)
	int32 MaxStack = SlotStacks[SlotIndex - 1];
	NewCount = FMath::Clamp(NewCount, 0, MaxStack);
	
	SlotCounts[SlotIndex - 1] = NewCount;
	
	UTextBlock* CountText = GetSlotCountText(SlotIndex);
	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::FromInt(NewCount)));
	}
}

void USmallShopWidget::UpdateSlotPrice(int32 SlotIndex, int32 NewPrice)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	SlotPrices[SlotIndex - 1] = NewPrice;
	
	UTextBlock* PriceText = GetSlotPriceText(SlotIndex);
	if (PriceText)
	{
		PriceText->SetText(FText::FromString(FString::FromInt(NewPrice)));
	}
}

void USmallShopWidget::UpdateSlotStack(int32 SlotIndex, int32 NewStack)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	SlotStacks[SlotIndex - 1] = NewStack;
	
	UTextBlock* StackText = GetSlotStackText(SlotIndex);
	if (StackText)
	{
		StackText->SetText(FText::FromString(FString::FromInt(NewStack)));
	}
}

void USmallShopWidget::UpdatePurchaseText(const FString& NewText)
{
	if (PurchaseText)
	{
		PurchaseText->SetText(FText::FromString(NewText));
	}
}

void USmallShopWidget::UpdateTotalPrice()
{
	int32 TotalPrice = 0;
	
	// 모든 슬롯의 Price * Count 합계 계산
	for (int32 i = 0; i < 5; i++)
	{
		TotalPrice += SlotPrices[i] * SlotCounts[i];
	}
	
	// PurchaseText 업데이트
	if (PurchaseText)
	{
		FString TotalText = FString::Printf(TEXT("총 금액: %d"), TotalPrice);
		PurchaseText->SetText(FText::FromString(TotalText));
	}
}

UTextBlock* USmallShopWidget::GetSlotCountText(int32 SlotIndex)
{
	switch (SlotIndex)
	{
	case 1: return Slot1Count;
	case 2: return Slot2Count;
	case 3: return Slot3Count;
	case 4: return Slot4Count;
	case 5: return Slot5Count;
	default: return nullptr;
	}
}

UButton* USmallShopWidget::GetSlotDownButton(int32 SlotIndex)
{
	switch (SlotIndex)
	{
	case 1: return Slot1Down;
	case 2: return Slot2Down;
	case 3: return Slot3Down;
	case 4: return Slot4Down;
	case 5: return Slot5Down;
	default: return nullptr;
	}
}

UButton* USmallShopWidget::GetSlotUpButton(int32 SlotIndex)
{
	switch (SlotIndex)
	{
	case 1: return Slot1Up;
	case 2: return Slot2Up;
	case 3: return Slot3Up;
	case 4: return Slot4Up;
	case 5: return Slot5Up;
	default: return nullptr;
	}
}

UTextBlock* USmallShopWidget::GetSlotPriceText(int32 SlotIndex)
{
	switch (SlotIndex)
	{
	case 1: return Slot1Price;
	case 2: return Slot2Price;
	case 3: return Slot3Price;
	case 4: return Slot4Price;
	case 5: return Slot5Price;
	default: return nullptr;
	}
}

UTextBlock* USmallShopWidget::GetSlotStackText(int32 SlotIndex)
{
	switch (SlotIndex)
	{
	case 1: return Slot1Stack;
	case 2: return Slot2Stack;
	case 3: return Slot3Stack;
	case 4: return Slot4Stack;
	case 5: return Slot5Stack;
	default: return nullptr;
	}
}
