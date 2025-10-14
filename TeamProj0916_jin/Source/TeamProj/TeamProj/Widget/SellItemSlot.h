#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SellItemSlot.generated.h"

class UItemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSellItemSlotClicked, FName, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSellItemSlotHovered, FName, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSellItemSlotUnhovered, FName, ItemID);

UCLASS()
class TEAMPROJ_API USellItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	USellItemSlot(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* SellItemButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* SellItemImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemSprite;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SellItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sell Item Slot")
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sell Item Slot")
	int32 ItemQuantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sell Item Slot")
	int32 SellPrice = 0;

	UPROPERTY()
	UItemComponent* CachedItemComponent;

	// 기본 색상 저장
	UPROPERTY()
	FLinearColor DefaultItemSpriteColor = FLinearColor::White;

	// 인벤토리 모드 여부 (true: 보유 수량만 표시, false: 판매/보유 수량 표시)
	UPROPERTY(BlueprintReadOnly, Category = "Sell Item Slot")
	bool bIsInventoryMode = false;

public:
	UPROPERTY(BlueprintAssignable, Category = "Sell Item Slot")
	FOnSellItemSlotClicked OnSellItemSlotClicked;

	UPROPERTY(BlueprintAssignable, Category = "Sell Item Slot")
	FOnSellItemSlotHovered OnSellItemSlotHovered;

	UPROPERTY(BlueprintAssignable, Category = "Sell Item Slot")
	FOnSellItemSlotUnhovered OnSellItemSlotUnhovered;

	UFUNCTION(BlueprintCallable, Category = "Sell Item Slot")
	void SetupSellItemSlot(UItemComponent* ItemComponent, FName InItemID, int32 InQuantity);

	UFUNCTION(BlueprintCallable, Category = "Sell Item Slot")
	void SetSellItemData(FName InItemID, int32 InQuantity, int32 InPrice);

	UFUNCTION(BlueprintCallable, Category = "Sell Item Slot")
	void UpdateQuantity(int32 NewQuantity);

	UFUNCTION(BlueprintPure, Category = "Sell Item Slot")
	FName GetItemID() const { return ItemID; }

	UFUNCTION(BlueprintPure, Category = "Sell Item Slot")
	int32 GetItemQuantity() const { return ItemQuantity; }

	UFUNCTION(BlueprintPure, Category = "Sell Item Slot")
	int32 GetSellPrice() const { return SellPrice; }

	// 보유 수량 확인
	UFUNCTION(BlueprintPure, Category = "Sell Item Slot")
	int32 GetAvailableQuantity() const;

	// 보유 아이템 모드 설정 (인벤토리 표시용)
	UFUNCTION(BlueprintCallable, Category = "Sell Item Slot")
	void SetInventoryMode(bool bInInventoryMode);

	// 수량 강제 업데이트 (외부에서 호출)
	UFUNCTION(BlueprintCallable, Category = "Sell Item Slot")
	void ForceUpdateQuantity();

	// ItemSprite 색상 변경 함수들
	UFUNCTION(BlueprintCallable, Category = "Sell Item Slot")
	void SetItemSpriteColor(FLinearColor Color);

	UFUNCTION(BlueprintCallable, Category = "Sell Item Slot")
	void ResetItemSpriteColor();

protected:
	UFUNCTION()
	void OnSellItemButtonClicked();

	UFUNCTION()
	void OnSellItemButtonHovered();

	UFUNCTION()
	void OnSellItemButtonUnhovered();

	UFUNCTION()
	void OnSellItemButtonPressed();
};
