// Fill out your copyright notice in the Description page of Project Settings.

#include "SmallShop.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "../Shop/ShopComponent.h"
#include "../Shop/ShopStruct.h"
#include "../Item/ItemComponent.h"
#include "../MyGamePlayerState.h"
#include "Kismet/GameplayStatics.h"

USmallShop::USmallShop(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Initialize slot arrays with default values
	SlotCounts.Init(0, 5);
	SlotPrices.Init(0, 5);
	SlotOriginalPrices.Init(0, 5);
	SlotStacks.Init(0, 5);
	SlotItemIDs.Init(NAME_None, 5);
	SlotHasDiscount.Init(false, 5);
	
	CachedShopComponent = nullptr;
	CachedItemComponent = nullptr;
}

void USmallShop::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind Purchase Button
	if (PurchaseButton)
	{
		PurchaseButton->OnClicked.AddDynamic(this, &USmallShop::OnPurchaseButtonClicked);
	}

	// Bind Slot 1 Buttons
	if (Slot1Down)
	{
		Slot1Down->OnClicked.AddDynamic(this, &USmallShop::OnSlot1DownClicked);
	}
	if (Slot1Up)
	{
		Slot1Up->OnClicked.AddDynamic(this, &USmallShop::OnSlot1UpClicked);
	}

	// Bind Slot 2 Buttons
	if (Slot2Down)
	{
		Slot2Down->OnClicked.AddDynamic(this, &USmallShop::OnSlot2DownClicked);
	}
	if (Slot2Up)
	{
		Slot2Up->OnClicked.AddDynamic(this, &USmallShop::OnSlot2UpClicked);
	}

	// Bind Slot 3 Buttons
	if (Slot3Down)
	{
		Slot3Down->OnClicked.AddDynamic(this, &USmallShop::OnSlot3DownClicked);
	}
	if (Slot3Up)
	{
		Slot3Up->OnClicked.AddDynamic(this, &USmallShop::OnSlot3UpClicked);
	}

	// Bind Slot 4 Buttons
	if (Slot4Down)
	{
		Slot4Down->OnClicked.AddDynamic(this, &USmallShop::OnSlot4DownClicked);
	}
	if (Slot4Up)
	{
		Slot4Up->OnClicked.AddDynamic(this, &USmallShop::OnSlot4UpClicked);
	}

	// Bind Slot 5 Buttons
	if (Slot5Down)
	{
		Slot5Down->OnClicked.AddDynamic(this, &USmallShop::OnSlot5DownClicked);
	}
	if (Slot5Up)
	{
		Slot5Up->OnClicked.AddDynamic(this, &USmallShop::OnSlot5UpClicked);
	}

	// Bind Item Slot Button Hover Events
	if (ItemSlot1Button)
	{
		ItemSlot1Button->OnHovered.AddDynamic(this, &USmallShop::OnItemSlot1ButtonHovered);
		ItemSlot1Button->OnUnhovered.AddDynamic(this, &USmallShop::OnItemSlot1ButtonUnhovered);
	}
	if (ItemSlot2Button)
	{
		ItemSlot2Button->OnHovered.AddDynamic(this, &USmallShop::OnItemSlot2ButtonHovered);
		ItemSlot2Button->OnUnhovered.AddDynamic(this, &USmallShop::OnItemSlot2ButtonUnhovered);
	}
	if (ItemSlot3Button)
	{
		ItemSlot3Button->OnHovered.AddDynamic(this, &USmallShop::OnItemSlot3ButtonHovered);
		ItemSlot3Button->OnUnhovered.AddDynamic(this, &USmallShop::OnItemSlot3ButtonUnhovered);
	}
	if (ItemSlot4Button)
	{
		ItemSlot4Button->OnHovered.AddDynamic(this, &USmallShop::OnItemSlot4ButtonHovered);
		ItemSlot4Button->OnUnhovered.AddDynamic(this, &USmallShop::OnItemSlot4ButtonUnhovered);
	}
	if (ItemSlot5Button)
	{
		ItemSlot5Button->OnHovered.AddDynamic(this, &USmallShop::OnItemSlot5ButtonHovered);
		ItemSlot5Button->OnUnhovered.AddDynamic(this, &USmallShop::OnItemSlot5ButtonUnhovered);
	}

	// Initialize default values
	
	// Initialize ItemDetail SizeBoxes to hidden
	if (ItemDetail1) ItemDetail1->SetVisibility(ESlateVisibility::Hidden);
	if (ItemDetail2) ItemDetail2->SetVisibility(ESlateVisibility::Hidden);
	if (ItemDetail3) ItemDetail3->SetVisibility(ESlateVisibility::Hidden);
	if (ItemDetail4) ItemDetail4->SetVisibility(ESlateVisibility::Hidden);
	if (ItemDetail5) ItemDetail5->SetVisibility(ESlateVisibility::Hidden);
	
	// Set default slot values
	for (int32 i = 0; i < 5; i++)
	{
		UpdateSlotCount(i + 1, 0);
		UpdateSlotPrice(i + 1, 99); // Default prices: 100, 200, 300, 400, 500
		UpdateSlotStack(i + 1, 1);
	}
	
	// Initialize total price
	UpdateTotalPrice();
}

void USmallShop::OnPurchaseButtonClicked()
{
	if (!CachedItemComponent || !CachedShopComponent)
	{
		return;
	}

	// 플레이어의 PlayerState 가져오기
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		return;
	}
	
	AMyGamePlayerState* PlayerState = Cast<AMyGamePlayerState>(PC->PlayerState);
	if (!PlayerState)
	{
		return;
	}

	// 총 구매 금액 계산
	int32 TotalCost = 0;
	for (int32 i = 0; i < 5; i++)
	{
		if (SlotCounts[i] > 0 && SlotItemIDs[i] != NAME_None)
		{
			TotalCost += SlotPrices[i] * SlotCounts[i];
		}
	}

	// 골드가 충분한지 확인
	if (PlayerState->Gold < TotalCost)
	{
		return; // 골드 부족
	}

	bool bAnyPurchase = false;
	
	// 각 슬롯의 수량만큼 구매 시도
	for (int32 i = 0; i < 5; i++)
	{
		if (SlotCounts[i] > 0 && SlotItemIDs[i] != NAME_None)
		{
			FName ItemID = SlotItemIDs[i];
			int32 Count = SlotCounts[i];
			
			// 플레이어의 ItemComponent에 아이템 추가
			if (CachedItemComponent->AddItemQuantity(ItemID, Count))
			{
				bAnyPurchase = true;
				
				// 상점 재고 감소
				CachedShopComponent->UpdateShopItemStock(ItemID, SlotStacks[i] - Count);
				
				// 구매 후 수량 초기화
				SlotCounts[i] = 0;
				UpdateSlotCount(i + 1, 0);
			}
		}
	}
	
	if (bAnyPurchase)
	{
		// 골드 차감
		PlayerState->Gold -= TotalCost;
		
		// 상점 데이터 새로고침 (재고 업데이트)
		RefreshShopData();
		
		// 총 가격 업데이트
		UpdateTotalPrice();
		
		// 구매 성공 델리게이트 호출
		OnPurchaseSuccess.Broadcast();
	}
}

void USmallShop::OnSlot1DownClicked()
{
	if (SlotCounts[0] > 0)
	{
		SlotCounts[0]--;
		UpdateSlotCount(1, SlotCounts[0]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot1UpClicked()
{
	if (SlotCounts[0] < SlotStacks[0])
	{
		// 플레이어의 아이템 컴포넌트에서 MaxQuantity 체크
		if (CachedItemComponent && SlotItemIDs[0] != NAME_None)
		{
			UItem* PlayerItem = CachedItemComponent->GetAllItem(SlotItemIDs[0]);
			if (PlayerItem)
			{
				FItemData ItemData = PlayerItem->GetItemData();
				int32 CurrentQuantity = CachedItemComponent->GetItemQuantity(SlotItemIDs[0]);
				
				// 현재 보유 수량 + 구매할 수량이 최대 수량을 초과하지 않는지 체크
				if (CurrentQuantity + SlotCounts[0] + 1 > ItemData.MaxQuantity)
				{
					return; // 최대 수량 초과로 증가 불가
				}
			}
		}
		
		SlotCounts[0]++;
		UpdateSlotCount(1, SlotCounts[0]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot2DownClicked()
{
	if (SlotCounts[1] > 0)
	{
		SlotCounts[1]--;
		UpdateSlotCount(2, SlotCounts[1]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot2UpClicked()
{
	if (SlotCounts[1] < SlotStacks[1])
	{
		// 플레이어의 아이템 컴포넌트에서 MaxQuantity 체크
		if (CachedItemComponent && SlotItemIDs[1] != NAME_None)
		{
			UItem* PlayerItem = CachedItemComponent->GetAllItem(SlotItemIDs[1]);
			if (PlayerItem)
			{
				FItemData ItemData = PlayerItem->GetItemData();
				int32 CurrentQuantity = CachedItemComponent->GetItemQuantity(SlotItemIDs[1]);
				
				// 현재 보유 수량 + 구매할 수량이 최대 수량을 초과하지 않는지 체크
				if (CurrentQuantity + SlotCounts[1] + 1 > ItemData.MaxQuantity)
				{
					return; // 최대 수량 초과로 증가 불가
				}
			}
		}
		
		SlotCounts[1]++;
		UpdateSlotCount(2, SlotCounts[1]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot3DownClicked()
{
	if (SlotCounts[2] > 0)
	{
		SlotCounts[2]--;
		UpdateSlotCount(3, SlotCounts[2]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot3UpClicked()
{
	if (SlotCounts[2] < SlotStacks[2])
	{
		// 플레이어의 아이템 컴포넌트에서 MaxQuantity 체크
		if (CachedItemComponent && SlotItemIDs[2] != NAME_None)
		{
			UItem* PlayerItem = CachedItemComponent->GetAllItem(SlotItemIDs[2]);
			if (PlayerItem)
			{
				FItemData ItemData = PlayerItem->GetItemData();
				int32 CurrentQuantity = CachedItemComponent->GetItemQuantity(SlotItemIDs[2]);
				
				// 현재 보유 수량 + 구매할 수량이 최대 수량을 초과하지 않는지 체크
				if (CurrentQuantity + SlotCounts[2] + 1 > ItemData.MaxQuantity)
				{
					return; // 최대 수량 초과로 증가 불가
				}
			}
		}
		
		SlotCounts[2]++;
		UpdateSlotCount(3, SlotCounts[2]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot4DownClicked()
{
	if (SlotCounts[3] > 0)
	{
		SlotCounts[3]--;
		UpdateSlotCount(4, SlotCounts[3]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot4UpClicked()
{
	if (SlotCounts[3] < SlotStacks[3])
	{
		// 플레이어의 아이템 컴포넌트에서 MaxQuantity 체크
		if (CachedItemComponent && SlotItemIDs[3] != NAME_None)
		{
			UItem* PlayerItem = CachedItemComponent->GetAllItem(SlotItemIDs[3]);
			if (PlayerItem)
			{
				FItemData ItemData = PlayerItem->GetItemData();
				int32 CurrentQuantity = CachedItemComponent->GetItemQuantity(SlotItemIDs[3]);
				
				// 현재 보유 수량 + 구매할 수량이 최대 수량을 초과하지 않는지 체크
				if (CurrentQuantity + SlotCounts[3] + 1 > ItemData.MaxQuantity)
				{
					return; // 최대 수량 초과로 증가 불가
				}
			}
		}
		
		SlotCounts[3]++;
		UpdateSlotCount(4, SlotCounts[3]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot5DownClicked()
{
	if (SlotCounts[4] > 0)
	{
		SlotCounts[4]--;
		UpdateSlotCount(5, SlotCounts[4]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnSlot5UpClicked()
{
	if (SlotCounts[4] < SlotStacks[4])
	{
		// 플레이어의 아이템 컴포넌트에서 MaxQuantity 체크
		if (CachedItemComponent && SlotItemIDs[4] != NAME_None)
		{
			UItem* PlayerItem = CachedItemComponent->GetAllItem(SlotItemIDs[4]);
			if (PlayerItem)
			{
				FItemData ItemData = PlayerItem->GetItemData();
				int32 CurrentQuantity = CachedItemComponent->GetItemQuantity(SlotItemIDs[4]);
				
				// 현재 보유 수량 + 구매할 수량이 최대 수량을 초과하지 않는지 체크
				if (CurrentQuantity + SlotCounts[4] + 1 > ItemData.MaxQuantity)
				{
					return; // 최대 수량 초과로 증가 불가
				}
			}
		}
		
		SlotCounts[4]++;
		UpdateSlotCount(5, SlotCounts[4]);
		UpdateTotalPrice();
	}
}

void USmallShop::OnItemSlot1ButtonHovered()
{
	if (ItemDetail1)
	{
		ItemDetail1->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void USmallShop::OnItemSlot1ButtonUnhovered()
{
	if (ItemDetail1)
	{
		ItemDetail1->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USmallShop::OnItemSlot2ButtonHovered()
{
	if (ItemDetail2)
	{
		ItemDetail2->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void USmallShop::OnItemSlot2ButtonUnhovered()
{
	if (ItemDetail2)
	{
		ItemDetail2->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USmallShop::OnItemSlot3ButtonHovered()
{
	if (ItemDetail3)
	{
		ItemDetail3->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void USmallShop::OnItemSlot3ButtonUnhovered()
{
	if (ItemDetail3)
	{
		ItemDetail3->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USmallShop::OnItemSlot4ButtonHovered()
{
	if (ItemDetail4)
	{
		ItemDetail4->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void USmallShop::OnItemSlot4ButtonUnhovered()
{
	if (ItemDetail4)
	{
		ItemDetail4->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USmallShop::OnItemSlot5ButtonHovered()
{
	if (ItemDetail5)
	{
		ItemDetail5->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void USmallShop::OnItemSlot5ButtonUnhovered()
{
	if (ItemDetail5)
	{
		ItemDetail5->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USmallShop::UpdateSlotCount(int32 SlotIndex, int32 NewCount)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	// Stack 범위 내에서만 Count 설정 (0 ~ Stack)
	int32 MaxStack = SlotStacks[SlotIndex - 1];
	NewCount = FMath::Clamp(NewCount, 0, MaxStack);
	
	SlotCounts[SlotIndex - 1] = NewCount;
	
	switch (SlotIndex)
	{
	case 1: if (Slot1Count) Slot1Count->SetText(FText::FromString(FString::FromInt(NewCount))); break;
	case 2: if (Slot2Count) Slot2Count->SetText(FText::FromString(FString::FromInt(NewCount))); break;
	case 3: if (Slot3Count) Slot3Count->SetText(FText::FromString(FString::FromInt(NewCount))); break;
	case 4: if (Slot4Count) Slot4Count->SetText(FText::FromString(FString::FromInt(NewCount))); break;
	case 5: if (Slot5Count) Slot5Count->SetText(FText::FromString(FString::FromInt(NewCount))); break;
	}
}

void USmallShop::UpdateSlotPrice(int32 SlotIndex, int32 NewPrice)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	SlotPrices[SlotIndex - 1] = NewPrice;
	
	// 할인 여부에 따라 가격 텍스트 결정
	FString PriceText;
	if (SlotHasDiscount[SlotIndex - 1])
	{
		PriceText = FString::Printf(TEXT("할인가: %d"), NewPrice);
	}
	else
	{
		PriceText = FString::Printf(TEXT("가격: %d"), NewPrice);
	}
	
	// 텍스트와 색상 모두 업데이트
	switch (SlotIndex)
	{
	case 1: 
		if (Slot1Price) 
		{
			Slot1Price->SetText(FText::FromString(PriceText));
			UpdateSlotPriceColor(1, SlotHasDiscount[0]);
		}
		break;
	case 2: 
		if (Slot2Price) 
		{
			Slot2Price->SetText(FText::FromString(PriceText));
			UpdateSlotPriceColor(2, SlotHasDiscount[1]);
		}
		break;
	case 3: 
		if (Slot3Price) 
		{
			Slot3Price->SetText(FText::FromString(PriceText));
			UpdateSlotPriceColor(3, SlotHasDiscount[2]);
		}
		break;
	case 4: 
		if (Slot4Price) 
		{
			Slot4Price->SetText(FText::FromString(PriceText));
			UpdateSlotPriceColor(4, SlotHasDiscount[3]);
		}
		break;
	case 5: 
		if (Slot5Price) 
		{
			Slot5Price->SetText(FText::FromString(PriceText));
			UpdateSlotPriceColor(5, SlotHasDiscount[4]);
		}
		break;
	}
}

void USmallShop::UpdateSlotOriginalPrice(int32 SlotIndex, int32 OriginalPrice)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	SlotOriginalPrices[SlotIndex - 1] = OriginalPrice;
}

void USmallShop::UpdateSlotDiscountInfo(int32 SlotIndex, bool bHasDiscount)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	SlotHasDiscount[SlotIndex - 1] = bHasDiscount;
	
	// 할인 정보가 변경되면 가격 표시도 다시 업데이트
	UpdateSlotPrice(SlotIndex, SlotPrices[SlotIndex - 1]);
}

void USmallShop::UpdateSlotStack(int32 SlotIndex, int32 NewStack)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	SlotStacks[SlotIndex - 1] = NewStack;
	
	FString StackText = FString::Printf(TEXT("재고: %d"), NewStack);
	switch (SlotIndex)
	{
	case 1: if (Slot1Stack) Slot1Stack->SetText(FText::FromString(StackText)); break;
	case 2: if (Slot2Stack) Slot2Stack->SetText(FText::FromString(StackText)); break;
	case 3: if (Slot3Stack) Slot3Stack->SetText(FText::FromString(StackText)); break;
	case 4: if (Slot4Stack) Slot4Stack->SetText(FText::FromString(StackText)); break;
	case 5: if (Slot5Stack) Slot5Stack->SetText(FText::FromString(StackText)); break;
	}
}

void USmallShop::UpdateSlotPriceColor(int32 SlotIndex, bool bIsDiscount)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	FSlateColor TextColor = bIsDiscount ? FSlateColor(FLinearColor::Red) : FSlateColor(FLinearColor::White);
	
	switch (SlotIndex)
	{
	case 1: if (Slot1Price) Slot1Price->SetColorAndOpacity(TextColor); break;
	case 2: if (Slot2Price) Slot2Price->SetColorAndOpacity(TextColor); break;
	case 3: if (Slot3Price) Slot3Price->SetColorAndOpacity(TextColor); break;
	case 4: if (Slot4Price) Slot4Price->SetColorAndOpacity(TextColor); break;
	case 5: if (Slot5Price) Slot5Price->SetColorAndOpacity(TextColor); break;
	}
}


void USmallShop::UpdateTotalPrice()
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

void USmallShop::SetupShop(UShopComponent* ShopComponent, UItemComponent* ItemComponent)
{
	if (!ShopComponent)
	{
		return;
	}

	if (!ItemComponent)
	{
		return;
	}

	CachedShopComponent = ShopComponent;
	CachedItemComponent = ItemComponent;
	
	// ShopComponent가 초기화되지 않았다면 초기화
	if (ShopComponent->GetAllShopItems().Num() == 0)
	{
		ShopComponent->InitAllShopItems();
	}
	
	// 상점 아이템들을 슬롯에 배치
	TArray<UShopItem*> ShopItems = ShopComponent->GetAllShopItems();
	
	// 모든 슬롯 초기화
	for (int32 i = 0; i < 5; i++)
	{
		SlotItemIDs[i] = NAME_None;
		SlotCounts[i] = 0;
		SlotPrices[i] = 0;
		SlotOriginalPrices[i] = 0;
		SlotStacks[i] = 0;
		SlotHasDiscount[i] = false;
		UpdateSlotCount(i + 1, 0);
		UpdateSlotPrice(i + 1, 0);
		UpdateSlotStack(i + 1, 0);
	}
	
	// 상점 아이템들을 슬롯에 배치 (최대 5개)
	int32 MaxSlots = FMath::Min(5, ShopItems.Num());
	
	for (int32 i = 0; i < MaxSlots; i++)
	{
		UShopItem* ShopItem = ShopItems[i];
		if (!ShopItem) 
		{
			continue;
		}
		
		FShopItemData ItemData = ShopItem->GetShopItemData();
		
		// 슬롯에 아이템 정보 설정
		SlotItemIDs[i] = ItemData.ItemID;
		
		// 가격과 재고를 배열과 UI에 모두 설정
		int32 CurrentPrice = ShopItem->GetCurrentPrice();
		int32 OriginalPrice = ItemData.Price; // 원래 가격
		int32 CurrentStock = ShopItem->GetCurrentStock();
		bool bHasDiscount = ItemData.DiscountInfo.bIsActive && ItemData.DiscountInfo.DiscountAmount > 0;
		
		SlotPrices[i] = CurrentPrice;
		SlotOriginalPrices[i] = OriginalPrice;
		SlotStacks[i] = CurrentStock;
		SlotHasDiscount[i] = bHasDiscount;
		
		UpdateSlotPrice(i + 1, CurrentPrice);
		UpdateSlotOriginalPrice(i + 1, OriginalPrice);
		UpdateSlotDiscountInfo(i + 1, bHasDiscount);
		UpdateSlotStack(i + 1, CurrentStock);
		
		// 아이템 이미지와 텍스트 설정
		UpdateItemSlotImage(i + 1, ItemData.Icon);
		UpdateItemDetailImage(i + 1, ItemData.Icon);
		UpdateItemDetailName(i + 1, ItemData.ItemName);
		UpdateItemDetailText(i + 1, ItemData.Description);
	}
	
	// 총 가격 업데이트
	UpdateTotalPrice();
}

void USmallShop::RefreshShopData()
{
	if (!CachedShopComponent)
	{
		return;
	}

	// 각 슬롯의 재고와 가격 업데이트
	for (int32 i = 0; i < 5; i++)
	{
		if (SlotItemIDs[i] != NAME_None)
		{
			FName ItemID = SlotItemIDs[i];
			
		// 현재 가격과 재고 가져오기
		int32 CurrentPrice = CachedShopComponent->GetShopItemPrice(ItemID);
		int32 CurrentStock = CachedShopComponent->GetShopItemStock(ItemID);
		
		// 아이템 정보도 업데이트 (아이콘, 이름, 설명, 할인 정보)
		UShopItem* ShopItem = CachedShopComponent->GetShopItem(ItemID);
		if (ShopItem)
		{
			FShopItemData ItemData = ShopItem->GetShopItemData();
			int32 OriginalPrice = ItemData.Price; // 원래 가격
			bool bHasDiscount = ItemData.DiscountInfo.bIsActive && ItemData.DiscountInfo.DiscountAmount > 0;
			
			// 배열과 UI 모두 업데이트
			SlotPrices[i] = CurrentPrice;
			SlotOriginalPrices[i] = OriginalPrice;
			SlotStacks[i] = CurrentStock;
			SlotHasDiscount[i] = bHasDiscount;
			
			UpdateSlotPrice(i + 1, CurrentPrice);
			UpdateSlotOriginalPrice(i + 1, OriginalPrice);
			UpdateSlotDiscountInfo(i + 1, bHasDiscount);
			UpdateSlotStack(i + 1, CurrentStock);
			
			// 아이템 정보도 업데이트 (아이콘, 이름, 설명)
			UpdateItemSlotImage(i + 1, ItemData.Icon);
			UpdateItemDetailImage(i + 1, ItemData.Icon);
			UpdateItemDetailName(i + 1, ItemData.ItemName);
			UpdateItemDetailText(i + 1, ItemData.Description);
		}
		else
		{
			// ShopItem이 없으면 기본 업데이트만
			SlotPrices[i] = CurrentPrice;
			SlotStacks[i] = CurrentStock;
			
			UpdateSlotPrice(i + 1, CurrentPrice);
			UpdateSlotStack(i + 1, CurrentStock);
		}
			
			// 현재 수량이 재고를 초과하면 조정
			if (SlotCounts[i] > CurrentStock)
			{
				SlotCounts[i] = CurrentStock;
				UpdateSlotCount(i + 1, CurrentStock);
			}
		}
	}
	
	// 총 가격 업데이트
	UpdateTotalPrice();
}

void USmallShop::UpdateItemSlotImage(int32 SlotIndex, UTexture2D* Icon)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	switch (SlotIndex)
	{
	case 1: if (ItemSlot1Image && Icon) ItemSlot1Image->SetBrushFromTexture(Icon); break;
	case 2: if (ItemSlot2Image && Icon) ItemSlot2Image->SetBrushFromTexture(Icon); break;
	case 3: if (ItemSlot3Image && Icon) ItemSlot3Image->SetBrushFromTexture(Icon); break;
	case 4: if (ItemSlot4Image && Icon) ItemSlot4Image->SetBrushFromTexture(Icon); break;
	case 5: if (ItemSlot5Image && Icon) ItemSlot5Image->SetBrushFromTexture(Icon); break;
	}
}

void USmallShop::UpdateItemDetailImage(int32 SlotIndex, UTexture2D* Icon)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	switch (SlotIndex)
	{
	case 1: if (ItemDetail1Image && Icon) ItemDetail1Image->SetBrushFromTexture(Icon); break;
	case 2: if (ItemDetail2Image && Icon) ItemDetail2Image->SetBrushFromTexture(Icon); break;
	case 3: if (ItemDetail3Image && Icon) ItemDetail3Image->SetBrushFromTexture(Icon); break;
	case 4: if (ItemDetail4Image && Icon) ItemDetail4Image->SetBrushFromTexture(Icon); break;
	case 5: if (ItemDetail5Image && Icon) ItemDetail5Image->SetBrushFromTexture(Icon); break;
	}
}

void USmallShop::UpdateItemDetailName(int32 SlotIndex, const FString& ItemName)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	switch (SlotIndex)
	{
	case 1: if (ItemDetail1ItemName) ItemDetail1ItemName->SetText(FText::FromString(ItemName)); break;
	case 2: if (ItemDetail2ItemName) ItemDetail2ItemName->SetText(FText::FromString(ItemName)); break;
	case 3: if (ItemDetail3ItemName) ItemDetail3ItemName->SetText(FText::FromString(ItemName)); break;
	case 4: if (ItemDetail4ItemName) ItemDetail4ItemName->SetText(FText::FromString(ItemName)); break;
	case 5: if (ItemDetail5ItemName) ItemDetail5ItemName->SetText(FText::FromString(ItemName)); break;
	}
}

void USmallShop::UpdateItemDetailText(int32 SlotIndex, const FString& Description)
{
	if (SlotIndex < 1 || SlotIndex > 5) return;

	switch (SlotIndex)
	{
	case 1: if (ItemDetail1ItemDetail) ItemDetail1ItemDetail->SetText(FText::FromString(Description)); break;
	case 2: if (ItemDetail2ItemDetail) ItemDetail2ItemDetail->SetText(FText::FromString(Description)); break;
	case 3: if (ItemDetail3ItemDetail) ItemDetail3ItemDetail->SetText(FText::FromString(Description)); break;
	case 4: if (ItemDetail4ItemDetail) ItemDetail4ItemDetail->SetText(FText::FromString(Description)); break;
	case 5: if (ItemDetail5ItemDetail) ItemDetail5ItemDetail->SetText(FText::FromString(Description)); break;
	}
}

