// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "SellShop.generated.h"

class UItemComponent;
class USellItemSlot;

// 판매 성공 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSellSuccess);

/**
 * 판매 기능 중심의 상점 위젯
 * 플레이어가 보유한 아이템을 판매할 수 있는 UI
 */
UCLASS()
class TEAMPROJ_API USellShop : public UUserWidget
{
	GENERATED_BODY()

public:
	USellShop(const FObjectInitializer& ObjectInitializer);

	// 판매 성공 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnSellSuccess OnSellSuccess;


protected:
	virtual void NativeConstruct() override;

public:
	// Buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* InButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OutButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SellButton;

	// Text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SellText;

	// Uniform Grid Panels
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SellSlotUniformGridPanel;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ItemSlotUniformGridPanel;

	// Item Detail Components
	UPROPERTY(meta = (BindWidget))
	class USizeBox* ItemDetail;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemDetailImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDetailQuantity;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDetailEx1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDetailEx2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDetailItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemDetailItemDetail;

	// Button Click Functions
	UFUNCTION()
	void OnInButtonClicked();

	UFUNCTION()
	void OnOutButtonClicked();

	UFUNCTION()
	void OnSellButtonClicked();

	// Item Component Integration
	UFUNCTION(BlueprintCallable)
	void SetupSellShop(UItemComponent* ItemComponent);

	UFUNCTION(BlueprintCallable)
	void RefreshSellData();

	// 아이템 슬롯 클릭 이벤트
	UFUNCTION()
	void OnItemSlotClicked(FName ItemID);

	// 선택된 아이템 ID 가져오기
	UFUNCTION(BlueprintPure, Category = "Sell Shop")
	FName GetSelectedItemID() const { return SelectedItemID; }

	// 판매 목록에서 아이템 제거
	UFUNCTION(BlueprintCallable, Category = "Sell Shop")
	void RemoveFromSellList(FName ItemID);

	// 판매 아이템 슬롯 클릭 이벤트
	UFUNCTION()
	void OnSellItemSlotClicked(FName ItemID);

private:
	// Item Component reference
	UPROPERTY()
	UItemComponent* CachedItemComponent;



	// 선택된 아이템 ID 저장
	UPROPERTY(BlueprintReadOnly, Category = "Sell Shop", meta = (AllowPrivateAccess = "true"))
	FName SelectedItemID = NAME_None;

	// 판매 목록에 있는 아이템 슬롯들
	UPROPERTY()
	TArray<USellItemSlot*> SellItemSlots;

	// 총 판매 가격
	UPROPERTY(BlueprintReadOnly, Category = "Sell Shop", meta = (AllowPrivateAccess = "true"))
	int32 TotalSellPrice = 0;

	// 총 판매 가격 업데이트
	void UpdateTotalSellPrice();

	// 판매 슬롯들 재배치
	void RearrangeSellSlots();

	// 판매 목록 초기화
	void ClearSellList();

	// ItemComponent의 OnItemQuantityChanged 델리게이트 바인딩
	void BindToItemQuantityChanged();

	// 아이템 수량 변경 시 호출되는 함수
	UFUNCTION()
	void OnItemQuantityChanged(FName ItemID);

	// 아이템 슬롯 호버 이벤트
	UFUNCTION()
	void OnSellItemSlotHovered(FName ItemID);

	UFUNCTION()
	void OnSellItemSlotUnhovered(FName ItemID);

	// 아이템 상세 정보 업데이트 함수들
	UFUNCTION(BlueprintCallable, Category = "Sell Shop")
	void UpdateItemDetail(FName ItemID);

	UFUNCTION(BlueprintCallable, Category = "Sell Shop")
	void HideItemDetail();

	

};