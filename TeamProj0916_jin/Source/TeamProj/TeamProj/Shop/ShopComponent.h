// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShopStruct.h"
#include "ShopComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopItemPurchased, FName, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopItemSold, FName, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopStockChanged, FName, ItemID);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAMPROJ_API UShopComponent : public UActorComponent
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere)
    TArray<UShopItem*> m_AllShopItems;  //상점 아이템 목록


    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 m_ShopLevel = 1;  //상점 레벨 (아이템 개수 결정)

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 m_RefreshCount = 1;  //새로고침 가능 횟수 (상점 레벨과 동일)

    UPROPERTY(BlueprintAssignable)
    FOnShopItemPurchased OnShopItemPurchased;  //아이템 구매 델리게이트

    UPROPERTY(BlueprintAssignable)
    FOnShopItemSold OnShopItemSold;  //아이템 판매 델리게이트

    UPROPERTY(BlueprintAssignable)
    FOnShopStockChanged OnShopStockChanged;  //재고 변경 델리게이트


public:
    UShopComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void InitAllShopItems();  //상점 목록 초기화

    UFUNCTION(BlueprintCallable)
    void RefreshShopItems();  //상점 새로고침 (랜덤 재선택)

    UFUNCTION(BlueprintCallable)
    void LevelUpShop();  //상점 레벨업 (아이템 개수 증가)
    
    UFUNCTION(BlueprintCallable)
    void SetShopLevel(int32 NewLevel);  //상점 레벨을 설정 <-- 매라운드 destroy되고 다시 생성될때 상점 레벨을 전달해주기 위해 얘 사용

    UFUNCTION(BlueprintCallable)
    bool PurchaseItem(FName ItemID, int32 Count = 1);  //구매

    UFUNCTION(BlueprintCallable)
    bool SellItem(FName ItemID, int32 Count = 1);  //판매

    UFUNCTION(BlueprintCallable)
    bool CanPurchaseItem(FName ItemID, int32 Count = 1) const;  //구매 가능 체크

    UFUNCTION(BlueprintCallable)
    bool CanSellItem(FName ItemID, int32 Count = 1) const;  //판매 가능 체크


    UFUNCTION(BlueprintCallable)
    void UpdateShopItemStock(FName ItemID, int32 NewStock);  //상점 아이템 재고 업데이트

    UFUNCTION(BlueprintCallable)
    void UpdateShopItemDiscount(FName ItemID, const FShopDiscountInfo& DiscountInfo);  //상점 아이템 할인 업데이트

    UFUNCTION(BlueprintPure)
    UShopItem* GetShopItem(FName ItemID) const;  //상점 아이템 가져오기

    UFUNCTION(BlueprintPure)
    TArray<UShopItem*> GetAllShopItems() const;  //모든 상점 아이템 가져오기기

    UFUNCTION(BlueprintPure)
    int32 GetShopItemPrice(FName ItemID) const;  //상점 아이템 구매 가격

    UFUNCTION(BlueprintPure)
    int32 GetShopItemSellPrice(FName ItemID) const;  //상점 아이템 판매 가격

    UFUNCTION(BlueprintPure)
    int32 GetShopItemStock(FName ItemID) const;  //상점 아이템 재고



    UFUNCTION(BlueprintPure)
    int32 GetShopLevel() const { return m_ShopLevel; }  //상점 레벨 가져오기

    UFUNCTION(BlueprintPure)
    int32 GetRefreshCount() const { return m_RefreshCount; }  //새로고침 가능 횟수 가져오기

    UFUNCTION(BlueprintCallable)
    bool CanRefreshShop() const;  //새로고침 가능 여부 체크

    UFUNCTION(BlueprintCallable)
    bool UseRefreshCount();  //새로고침 횟수 사용
};
