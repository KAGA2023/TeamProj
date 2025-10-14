// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopComponent.h"
#include "../Character/GameCharacterBase.h"
#include "../Item/ItemComponent.h"
#include "../Status/StatusComponent.h"

UShopComponent::UShopComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    m_AllShopItems.Empty();
    m_ShopLevel = 1;
    m_RefreshCount = 1;
}

void UShopComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAllShopItems();
}

void UShopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShopComponent::InitAllShopItems()
{
    UDataTable* ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Item/DT_Item.DT_Item"));
    if (!ItemDataTable)
        return;

    TArray<FItemData*> AllRows;
    ItemDataTable->GetAllRows(TEXT("ItemInit"), AllRows);

    TArray<FItemData*> ShopItems;
    for (FItemData* Row : AllRows)
    {
        if (Row)
        {
            ShopItems.Add(Row);
        }
    }

    //섞어
    for (int32 i = ShopItems.Num() - 1; i > 0; i--)
    {
        int32 j = FMath::RandRange(0, i);
        ShopItems.Swap(i, j);
    }

    //상점 레벨에 따라 아이템 개수 바꿔 (시작 2개, 레벨업할때마다 1씩 증가)
    int32 ShopItemCount = FMath::Min(m_ShopLevel + 1, ShopItems.Num());
    for (int32 i = 0; i < ShopItemCount; i++)
    {
        FItemData* ItemData = ShopItems[i];
        if (!ItemData) continue;

        //ItemData -> ShopItemData 넣기
        FShopItemData ShopItemData;
        ShopItemData.ItemID = ItemData->ItemID;
        ShopItemData.ItemName = ItemData->ItemName;
        ShopItemData.Description = ItemData->Description;
        ShopItemData.Icon = ItemData->Icon;
        ShopItemData.Price = ItemData->Cost;
        ShopItemData.ItemData = ItemData;
        
        //랜덤 할인 설정 (30% 확률로 할인, 1원 아이템은 할인 제외)
        if (FMath::RandRange(0, 100) < 30 && ItemData->Cost > 1)
        {
            FShopDiscountInfo DiscountInfo;
            DiscountInfo.bIsActive = true;
            
            //고정 금액 할인 (가격의 10% ~ 50% 범위, 소수점 버림)
            int32 MinDiscount = FMath::FloorToInt(ItemData->Cost * 0.1f);
            int32 MaxDiscount = FMath::FloorToInt(ItemData->Cost * 0.5f);
            
            // 최소 1원 이상 할인되도록 보장
            if (MinDiscount < 1) MinDiscount = 1;
            if (MaxDiscount < MinDiscount) MaxDiscount = MinDiscount;
            
            DiscountInfo.DiscountAmount = FMath::RandRange(MinDiscount, MaxDiscount);
            
            ShopItemData.DiscountInfo = DiscountInfo;
        }

        UShopItem* NewShopItem = NewObject<UShopItem>(this);
        if (!NewShopItem) continue;

        NewShopItem->SetShopItemData(ShopItemData);
        
        //랜덤 재고 (5~10개)
        int32 RandomStock = FMath::RandRange(5, 10);
        NewShopItem->UpdateStock(RandomStock);
        
        m_AllShopItems.Add(NewShopItem);
    }
}

void UShopComponent::RefreshShopItems()
{
    m_AllShopItems.Empty();
    
    InitAllShopItems();
}

void UShopComponent::LevelUpShop()
{
    m_ShopLevel++;
    m_RefreshCount = m_ShopLevel;  //레벨업할 때마다 새로고침 횟수도 증가
    
    //레벨업 했으니 새로고침?
    RefreshShopItems();
}

void UShopComponent::SetShopLevel(int32 NewLevel)
{
    m_ShopLevel = NewLevel;
    m_RefreshCount = m_ShopLevel;  //새로고침 횟수도 레벨에 맞게 설정
    
    //상점 새로고침
    RefreshShopItems();
}

bool UShopComponent::PurchaseItem(FName ItemID, int32 Count)
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    if (!ShopItem) return false;

    AGameCharacterBase* Character = Cast<AGameCharacterBase>(GetOwner());
    if (!Character) return false;

    if (!ShopItem->CanPurchase(Character, Count)) return false;

    int32 CurrentPrice = ShopItem->GetCurrentPrice();
    int32 TotalCost = CurrentPrice * Count;

    if (ShopItem->PurchaseItem(Character, Count))
    {
        OnShopItemPurchased.Broadcast(ItemID);
        OnShopStockChanged.Broadcast(ItemID);

        return true;
    }

    return false;
}

bool UShopComponent::SellItem(FName ItemID, int32 Count)
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    if (!ShopItem) return false;

    AGameCharacterBase* Character = Cast<AGameCharacterBase>(GetOwner());
    if (!Character) return false;

    if (!ShopItem->CanSellItem(Character, ItemID, Count)) return false;

    int32 SellPrice = ShopItem->GetSellPrice(ItemID);
    int32 TotalEarned = SellPrice * Count;

    if (ShopItem->SellItem(Character, ItemID, Count))
    {
        OnShopItemSold.Broadcast(ItemID);

        return true;
    }

    return false;
}

bool UShopComponent::CanPurchaseItem(FName ItemID, int32 Count) const
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    if (!ShopItem) return false;

    AGameCharacterBase* Character = Cast<AGameCharacterBase>(GetOwner());
    if (!Character) return false;

    return ShopItem->CanPurchase(Character, Count);
}

bool UShopComponent::CanSellItem(FName ItemID, int32 Count) const
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    if (!ShopItem) return false;

    AGameCharacterBase* Character = Cast<AGameCharacterBase>(GetOwner());
    if (!Character) return false;

    return ShopItem->CanSellItem(Character, ItemID, Count);
}


void UShopComponent::UpdateShopItemStock(FName ItemID, int32 NewStock)
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    if (ShopItem)
    {
        ShopItem->UpdateStock(NewStock);
        OnShopStockChanged.Broadcast(ItemID);
    }
}

void UShopComponent::UpdateShopItemDiscount(FName ItemID, const FShopDiscountInfo& DiscountInfo)
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    if (ShopItem)
    {
        ShopItem->UpdateDiscount(DiscountInfo);
    }
}

UShopItem* UShopComponent::GetShopItem(FName ItemID) const
{
    for (UShopItem* ShopItem : m_AllShopItems)
    {
        if (ShopItem && ShopItem->GetShopItemData().ItemID == ItemID)
        {
            return ShopItem;
        }
    }
    return nullptr;
}

TArray<UShopItem*> UShopComponent::GetAllShopItems() const
{
    return m_AllShopItems;
}

int32 UShopComponent::GetShopItemPrice(FName ItemID) const
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    return ShopItem ? ShopItem->GetCurrentPrice() : 0;
}

int32 UShopComponent::GetShopItemSellPrice(FName ItemID) const
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    if (ShopItem)
    {
        return ShopItem->GetSellPrice(ItemID);
    }
    
    // 상점에 등록되지 않은 아이템의 경우 데이터테이블에서 기본 가격을 가져와서 50%로 계산
    UDataTable* ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Item/DT_Item.DT_Item"));
    if (ItemDataTable)
    {
        FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, TEXT("GetShopItemSellPrice"));
        if (ItemData)
        {
            return FMath::Max(1, ItemData->Cost / 2); // 구매 가격의 50%, 최소 1골드
        }
    }
    
    return 1; // 기본값
}

int32 UShopComponent::GetShopItemStock(FName ItemID) const
{
    UShopItem* ShopItem = GetShopItem(ItemID);
    return ShopItem ? ShopItem->GetCurrentStock() : 0;
}

bool UShopComponent::CanRefreshShop() const
{
    return m_RefreshCount > 0;
}

bool UShopComponent::UseRefreshCount()
{
    if (m_RefreshCount > 0)
    {
        m_RefreshCount--;
        return true;
    }
    return false;
}


