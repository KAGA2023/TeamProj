#include "ItemSlot.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../Item/ItemComponent.h"

UItemSlot::UItemSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 이벤트 바인딩
	if (ItemSlotButton)
	{
		ItemSlotButton->OnHovered.AddDynamic(this, &UItemSlot::OnItemSlotButtonHovered);
		ItemSlotButton->OnUnhovered.AddDynamic(this, &UItemSlot::OnItemSlotButtonUnhovered);
		ItemSlotButton->OnClicked.AddDynamic(this, &UItemSlot::OnItemSlotButtonClicked);
	}

	// 초기 설정
	if (ItemDetail)
	{
		ItemDetail->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemSlot::OnItemSlotButtonHovered()
{
	if (ItemDetail)
	{
		ItemDetail->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UItemSlot::OnItemSlotButtonUnhovered()
{
	if (ItemDetail)
	{
		ItemDetail->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemSlot::OnItemSlotButtonClicked()
{
	if (EnableClick)
	{
		OnItemSlotClicked.Broadcast(ItemID);
	}
}

void UItemSlot::SetupItemSlot(UItemComponent* ItemComponent)
{
	if (!ItemComponent)
	{
		return;
	}

	// 이전 바인딩 해제
	if (CachedItemComponent)
	{
		CachedItemComponent->OnItemQuantityChanged.RemoveDynamic(this, &UItemSlot::ChangeQuantity);
	}

	// 새 아이템 컴포넌트 캐시 및 바인딩
	CachedItemComponent = ItemComponent;
	CachedItemComponent->OnItemQuantityChanged.AddDynamic(this, &UItemSlot::ChangeQuantity);

	UItem* Item = ItemComponent->GetAllItem(ItemID);
	if (!Item)
	{
		return;
	}

	FItemData ItemData = Item->GetItemData();

	// 아이템 이미지 설정
	if (ItemSlotImage && ItemData.Icon)
	{
		ItemSlotImage->SetBrushFromTexture(ItemData.Icon);
	}

	if (ItemDetailImage && ItemData.Icon)
	{
		ItemDetailImage->SetBrushFromTexture(ItemData.Icon);
	}

	// 아이템 이름 설정
	if (ItemDetailItemName)
	{
		ItemDetailItemName->SetText(FText::FromString(ItemData.ItemName));
	}

	// 아이템 설명 설정
	if (ItemDetailItemDetail)
	{
		ItemDetailItemDetail->SetText(FText::FromString(ItemData.Description));
	}

	// 수량에 따른 EnableClick 설정
	EnableClick = (ItemQuantity > 0);

	// 수량 설정
	if (ItemSlotQuantity)
	{
		ItemSlotQuantity->SetText(FText::AsNumber(ItemQuantity));
	}

	if (ItemDetailQuantity)
	{
		ItemDetailQuantity->SetText(FText::AsNumber(ItemQuantity));
	}
}

void UItemSlot::SetItemSlotData(FName NewItemID, int32 NewQuantity)
{
	ItemID = NewItemID;
	ItemQuantity = NewQuantity;
}

void UItemSlot::ChangeQuantity(FName ChangedItemID)
{
	// 변경된 아이템이 현재 슬롯의 아이템과 같은지 확인
	if (ChangedItemID != ItemID || !CachedItemComponent)
	{
		return;
	}

	// 새로운 수량 가져오기
	ItemQuantity = CachedItemComponent->GetItemQuantity(ItemID);

	// 수량에 따른 EnableClick 설정
	EnableClick = (ItemQuantity > 0);

	// 수량 텍스트 업데이트
	if (ItemSlotQuantity)
	{
		ItemSlotQuantity->SetText(FText::AsNumber(ItemQuantity));
	}

	if (ItemDetailQuantity)
	{
		ItemDetailQuantity->SetText(FText::AsNumber(ItemQuantity));
	}
}
