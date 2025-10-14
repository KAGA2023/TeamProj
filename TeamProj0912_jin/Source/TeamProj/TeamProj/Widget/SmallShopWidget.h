// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SmallShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API USmallShopWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USmallShopWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	// Purchase Button and Text
	UPROPERTY(meta = (BindWidget))
	class UButton* PurchaseButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PurchaseText;

	// Slot 1 Components
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot1Count;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot1Down;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot1Up;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot1Price;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot1Stack;

	// Slot 2 Components
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot2Count;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot2Down;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot2Up;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot2Price;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot2Stack;

	// Slot 3 Components
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot3Count;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot3Down;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot3Up;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot3Price;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot3Stack;

	// Slot 4 Components
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot4Count;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot4Down;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot4Up;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot4Price;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot4Stack;

	// Slot 5 Components
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot5Count;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot5Down;

	UPROPERTY(meta = (BindWidget))
	class UButton* Slot5Up;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot5Price;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Slot5Stack;

	// Button Click Functions
	UFUNCTION()
	void OnPurchaseButtonClicked();

	UFUNCTION()
	void OnSlot1DownClicked();

	UFUNCTION()
	void OnSlot1UpClicked();

	UFUNCTION()
	void OnSlot2DownClicked();

	UFUNCTION()
	void OnSlot2UpClicked();

	UFUNCTION()
	void OnSlot3DownClicked();

	UFUNCTION()
	void OnSlot3UpClicked();

	UFUNCTION()
	void OnSlot4DownClicked();

	UFUNCTION()
	void OnSlot4UpClicked();

	UFUNCTION()
	void OnSlot5DownClicked();

	UFUNCTION()
	void OnSlot5UpClicked();

	// Slot Management Functions
	UFUNCTION(BlueprintCallable)
	void UpdateSlotCount(int32 SlotIndex, int32 NewCount);

	UFUNCTION(BlueprintCallable)
	void UpdateSlotPrice(int32 SlotIndex, int32 NewPrice);

	UFUNCTION(BlueprintCallable)
	void UpdateSlotStack(int32 SlotIndex, int32 NewStack);

	UFUNCTION(BlueprintCallable)
	void UpdatePurchaseText(const FString& NewText);

	UFUNCTION(BlueprintCallable)
	void UpdateTotalPrice();

private:
	// Internal slot data
	TArray<int32> SlotCounts;
	TArray<int32> SlotPrices;
	TArray<int32> SlotStacks;

	// Helper function to get slot components
	UTextBlock* GetSlotCountText(int32 SlotIndex);
	UButton* GetSlotDownButton(int32 SlotIndex);
	UButton* GetSlotUpButton(int32 SlotIndex);
	UTextBlock* GetSlotPriceText(int32 SlotIndex);
	UTextBlock* GetSlotStackText(int32 SlotIndex);
};
