// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemStruct.h"
#include "ItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemReady);    //아이템 준비 완료 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemQuantityChanged, FName, ItemID);    //아이템 수량 변경 델리게이트

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAMPROJ_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnyWhere)					TArray<UItem*> m_AllItems;  //모든 아이템 목록

	UPROPERTY(EditAnyWhere)					UItem* m_EquippedItem;  //장착된 아이템



public:
	UItemComponent();

	UPROPERTY(BlueprintAssignable)				FOnItemReady OnItemReady;
	UPROPERTY(BlueprintAssignable)				FOnItemQuantityChanged OnItemQuantityChanged;

protected:
	virtual void BeginPlay() override;


public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void InitAllItems();  //모든 아이템 목록 초기화

	UFUNCTION(BlueprintCallable)
	bool EquipItem(FName ItemID);  //장착

	UFUNCTION(BlueprintCallable)
	bool UnequipItem();  //해제

	UFUNCTION(BlueprintCallable)
	bool ChangeItem(FName ItemID);  //해제 후 장착

	UFUNCTION(BlueprintCallable)
	bool TryUseItem(AActor* OptionalTarget = nullptr, FVector OptionalLocation = FVector::ZeroVector);  //아이템 사용

	UFUNCTION(BlueprintCallable)
	bool AddItemQuantity(FName ItemID, int32 Amount);  //수량 증가

	UFUNCTION(BlueprintCallable)
	bool RemoveItemQuantity(FName ItemID, int32 Amount);  //수량 감소

	UFUNCTION(BlueprintPure)
	UItem* GetEquippedItem() const;  //장착된 아이템 UItem 포인터로 반환

	UFUNCTION(BlueprintPure)
	FName GetEquippedItemID() const;  //장착된 아이템 ItemID 반환

	UFUNCTION(BlueprintPure)
	UItem* GetAllItem(FName ItemID) const;  //모든 아이템 목록에서 ItemID 검색하여 UItem 포인터로 반환

	UFUNCTION(BlueprintPure)
	TArray<UItem*> GetAllItemData() const { return m_AllItems; }  //모든 아이템 목록의 모든 UItem 포인터로 반환

	UFUNCTION(BlueprintPure)
	int32 GetItemQuantity(FName ItemID) const;  //모든 아이템 목록에서 ItemID의 수량 반환

	UFUNCTION(BlueprintPure)
	int32 GetItemPrice(FName ItemID) const;  //모든 아이템 목록에서 ItemID의 가격 반환

	UFUNCTION(BlueprintPure)
	int32 GetItemSellPrice(FName ItemID) const;  //모든 아이템 목록에서 ItemID의 판매가 반환 (원래 가격 50%, 최소 1원)

	UFUNCTION(BlueprintPure)
	bool HasEquippedItem() const;  //장착된 아이템이 nullptr인지 체크 //true: 장착됨 //false: nullptr

	UFUNCTION(BlueprintPure)
	bool HasQuantity(FName ItemID) const;  //수량이 1 이상인지 체크
};
