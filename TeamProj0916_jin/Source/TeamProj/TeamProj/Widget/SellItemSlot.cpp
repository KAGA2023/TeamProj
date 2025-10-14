#include "SellItemSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../Item/ItemComponent.h"

USellItemSlot::USellItemSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemID = NAME_None;
	ItemQuantity = 0;
	SellPrice = 0;
	CachedItemComponent = nullptr;
	bIsInventoryMode = false;
	DefaultItemSpriteColor = FLinearColor::White;
}

void USellItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 이벤트 바인딩
	if (SellItemButton)
	{
		SellItemButton->OnClicked.AddDynamic(this, &USellItemSlot::OnSellItemButtonClicked);
		SellItemButton->OnHovered.AddDynamic(this, &USellItemSlot::OnSellItemButtonHovered);
		SellItemButton->OnUnhovered.AddDynamic(this, &USellItemSlot::OnSellItemButtonUnhovered);
		SellItemButton->OnPressed.AddDynamic(this, &USellItemSlot::OnSellItemButtonPressed);
	}

	// ItemSprite 기본 색상 저장
	if (ItemSprite)
	{
		DefaultItemSpriteColor = ItemSprite->GetColorAndOpacity();
	}
}

void USellItemSlot::SetupSellItemSlot(UItemComponent* ItemComponent, FName InItemID, int32 InQuantity)
{
	if (!ItemComponent)
	{
		return;
	}

	CachedItemComponent = ItemComponent;
	
	// 아이템 데이터 가져오기
	UItem* Item = ItemComponent->GetAllItem(InItemID);
	if (!Item)
	{
		return;
	}

	FItemData ItemData = Item->GetItemData();
	
	// 아이템 정보 설정
	ItemID = InItemID;
	ItemQuantity = InQuantity;
	
	// 데이터테이블에서 직접 판매 가격 계산 (구매 가격의 50%)
	SellPrice = FMath::Max(1, ItemData.Cost / 2); // 최소 1골드

	// UI 업데이트
	SetSellItemData(ItemID, ItemQuantity, SellPrice);
}

void USellItemSlot::SetSellItemData(FName InItemID, int32 InQuantity, int32 InPrice)
{
	ItemID = InItemID;
	ItemQuantity = InQuantity;
	SellPrice = InPrice;

	// 텍스트 업데이트 (모드에 따라 다르게 표시)
	if (SellItemQuantity)
	{
		int32 AvailableQty = GetAvailableQuantity();
		FString QuantityText;
		
		if (bIsInventoryMode)
		{
			// 인벤토리 모드: 보유 수량만 표시
			QuantityText = FString::FromInt(AvailableQty);
		}
		else
		{
			// 판매 모드: 판매 수량만 표시
			QuantityText = FString::FromInt(ItemQuantity);
		}
		
		SellItemQuantity->SetText(FText::FromString(QuantityText));
	}


	// 아이템 아이콘 설정 (필요시)
	if (CachedItemComponent)
	{
		UItem* Item = CachedItemComponent->GetAllItem(ItemID);
		if (Item && SellItemImage)
		{
			FItemData ItemData = Item->GetItemData();
			if (ItemData.Icon)
			{
				SellItemImage->SetBrushFromTexture(ItemData.Icon);
			}
		}
	}
}

void USellItemSlot::UpdateQuantity(int32 NewQuantity)
{
	ItemQuantity = NewQuantity;
	if (SellItemQuantity)
	{
		int32 AvailableQty = GetAvailableQuantity();
		FString QuantityText;
		
		if (bIsInventoryMode)
		{
			// 인벤토리 모드: 보유 수량만 표시
			QuantityText = FString::FromInt(AvailableQty);
		}
		else
		{
			// 판매 모드: 판매 수량만 표시
			QuantityText = FString::FromInt(ItemQuantity);
		}
		
		SellItemQuantity->SetText(FText::FromString(QuantityText));
	}
}

int32 USellItemSlot::GetAvailableQuantity() const
{
	if (CachedItemComponent)
	{
		return CachedItemComponent->GetItemQuantity(ItemID);
	}
	return 0;
}

void USellItemSlot::SetInventoryMode(bool bInInventoryMode)
{
	this->bIsInventoryMode = bInInventoryMode;
	
	// 모드 변경 시 수량 텍스트 업데이트
	if (SellItemQuantity)
	{
		int32 AvailableQty = GetAvailableQuantity();
		FString QuantityText;
		
		if (bIsInventoryMode)
		{
			// 인벤토리 모드: 보유 수량만 표시
			QuantityText = FString::FromInt(AvailableQty);
		}
		else
		{
			// 판매 모드: 판매 수량만 표시
			QuantityText = FString::FromInt(ItemQuantity);
		}
		
		SellItemQuantity->SetText(FText::FromString(QuantityText));
	}
}

void USellItemSlot::ForceUpdateQuantity()
{
	if (SellItemQuantity)
	{
		int32 AvailableQty = GetAvailableQuantity();
		FString QuantityText;
		
		if (bIsInventoryMode)
		{
			// 인벤토리 모드: 보유 수량만 표시
			QuantityText = FString::FromInt(AvailableQty);
		}
		else
		{
			// 판매 모드: 판매 수량만 표시
			QuantityText = FString::FromInt(ItemQuantity);
		}
		
		SellItemQuantity->SetText(FText::FromString(QuantityText));
	}
}

void USellItemSlot::OnSellItemButtonClicked()
{
	// 클릭 이벤트 브로드캐스트
	OnSellItemSlotClicked.Broadcast(ItemID);
}

void USellItemSlot::OnSellItemButtonHovered()
{
	// 호버 시 주황색으로 변경
	SetItemSpriteColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f)); // 주황색
	
	// 호버 이벤트 브로드캐스트
	OnSellItemSlotHovered.Broadcast(ItemID);
}

void USellItemSlot::OnSellItemButtonPressed()
{
	// 프레스 시 빨간색으로 변경
	SetItemSpriteColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)); // 빨간색
}

void USellItemSlot::OnSellItemButtonUnhovered()
{
	// 언호버 시 기본 색상으로 복원
	ResetItemSpriteColor();
	
	// 언호버 이벤트 브로드캐스트
	OnSellItemSlotUnhovered.Broadcast(ItemID);
}

void USellItemSlot::SetItemSpriteColor(FLinearColor Color)
{
	if (ItemSprite)
	{
		ItemSprite->SetColorAndOpacity(Color);
	}
}

void USellItemSlot::ResetItemSpriteColor()
{
	if (ItemSprite)
	{
		ItemSprite->SetColorAndOpacity(DefaultItemSpriteColor);
	}
}
