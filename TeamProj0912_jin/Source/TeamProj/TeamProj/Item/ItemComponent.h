// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemStruct.h"
#include "ItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemQuantityChanged, FName, ItemID);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAMPROJ_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnyWhere)					TArray<UItem*> m_AllItems;  //데이터테이블

	UPROPERTY(EditAnyWhere)					UItem* m_EquippedItem;  //장착된 아이템

	

public:
	UItemComponent();

	UPROPERTY(BlueprintAssignable)				FOnItemReady OnItemReady;  //아이템 준비여부 델리게이트  //수정
	UPROPERTY(BlueprintAssignable)				FOnItemQuantityChanged OnItemQuantityChanged;  //아이템 수량 변화 델리게이트  //수정

protected:
	virtual void BeginPlay() override;


public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void InitAllItems();  //데이터테이블 초기화

	UFUNCTION(BlueprintCallable)
	bool EquipItem(FName ItemID);  //장착

	UFUNCTION(BlueprintCallable)
	bool UnequipItem();  //탈착

	UFUNCTION(BlueprintCallable)
	bool ChangeItem(FName ItemID);  //탈착 후 장착

	UFUNCTION(BlueprintCallable)
	bool TryUseItem(AActor* OptionalTarget = nullptr, FVector OptionalLocation = FVector::ZeroVector);  //아이템 사용

	UFUNCTION(BlueprintCallable)
	bool AddItemQuantity(FName ItemID, int32 Amount);  //수량Up

	UFUNCTION(BlueprintCallable)
	bool RemoveItemQuantity(FName ItemID, int32 Amount);  //수량Down

	UFUNCTION(BlueprintPure)
	UItem* GetEquippedItem() const;  //장착된 아이템 UItem형식으로 반환

	UFUNCTION(BlueprintPure)
	FName GetEquippedItemID() const;  //장착된 아이템 ItemID반환

	UFUNCTION(BlueprintPure)
	UItem* GetAllItem(FName ItemID) const;  //데이터테이블에서 ItemID 같으면 UItem형식으로 반환

	UFUNCTION(BlueprintPure)
	TArray<UItem*> GetAllItemData() const { return m_AllItems; }  //데이터테이블에서 전부 UItem형식으로 반환

	UFUNCTION(BlueprintPure)
	int32 GetItemQuantity(FName ItemID) const;  //데이터테이블에서 ItemID로 수량 반환

	UFUNCTION(BlueprintPure)
	int32 GetItemPrice(FName ItemID) const;  //데이터테이블에서 ItemID로 가격 반환

	UFUNCTION(BlueprintPure)
	bool HasEquippedItem() const;  //현재 장착된 아이템이 nullptr인지 체크 //true: 장착된거 있음  //false: nullptr

	UFUNCTION(BlueprintPure)
	bool HasQuantity(FName ItemID) const;  //수량이 1이상인지 체크
};
