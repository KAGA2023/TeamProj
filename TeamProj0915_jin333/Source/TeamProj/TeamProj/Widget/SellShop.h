// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "SellShop.generated.h"

class UItemComponent;

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

private:
	// Item Component reference
	UPROPERTY()
	UItemComponent* CachedItemComponent;

};