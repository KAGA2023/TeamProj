#include "Inventory.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "../Item/ItemComponent.h"
#include "ItemSlot.h"

UInventory::UInventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedItemComponent = nullptr;
}

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventory::SetupInventory(UItemComponent* ItemComponent)
{
	if (!ItemComponent || !ItemSlotUniformGridPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupInventory: ItemComponent or ItemSlotUniformGridPanel is null"));
		return;
	}

	CachedItemComponent = ItemComponent;

	// 기존 슬롯들 제거
	ItemSlotUniformGridPanel->ClearChildren();

	// 모든 아이템 데이터 가져오기
	TArray<UItem*> AllItems = ItemComponent->GetAllItemData();

	// 아이템 슬롯 생성 및 배치
	for (int32 i = 0; i < AllItems.Num(); ++i)
	{
		UItem* Item = AllItems[i];
		if (!Item)
		{
			continue;
		}

		FItemData ItemData = Item->GetItemData();
		int32 ItemQuantity = ItemComponent->GetItemQuantity(ItemData.ItemID);

		// ItemSlot 위젯 생성 (W_ItemSlot 클래스 사용)
		UClass* ItemSlotClass = LoadClass<UItemSlot>(nullptr, TEXT("/Game/Widget/W_ItemSlot.W_ItemSlot_C"));
		UItemSlot* ItemSlot = CreateWidget<UItemSlot>(this, ItemSlotClass);
		if (ItemSlot)
		{
			// 위젯 크기 설정 (디버깅용)
			//ItemSlot->SetDesiredSizeInViewport(FVector2D(70.0f, 70.0f));

			// 그리드 위치 계산 (5열 기준)
			int32 Row = i / 5;
			int32 Column = i % 5;


			// 그리드 패널에 추가
			ItemSlotUniformGridPanel->AddChildToUniformGrid(ItemSlot, Row, Column);

			// ItemSlot의 ItemID와 ItemQuantity 설정
			ItemSlot->SetItemSlotData(ItemData.ItemID, ItemQuantity);

			// ItemSlot 설정
			ItemSlot->SetupItemSlot(ItemComponent);

			// 위젯 가시성 확인
			ItemSlot->SetVisibility(ESlateVisibility::Visible);

			// 아이템 슬롯 클릭 델리게이트 바인딩
			ItemSlot->OnItemSlotClicked.AddDynamic(this, &UInventory::ItemSlotClicked);

		}
	}
}

void UInventory::ItemSlotClicked(FName ItemID)
{
	// 플레이어의 ItemComponent에서 아이템 변경
	if (CachedItemComponent)
	{
		CachedItemComponent->ChangeItem(ItemID);
	}

	// 인벤토리 닫기 이벤트 호출
	OnInventoryClose.Broadcast();
}

