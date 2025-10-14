// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemStruct.h"
#include "ItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemReady);    //������ �غ� �Ϸ� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemQuantityChanged, FName, ItemID);    //������ ���� ���� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDataChanged, FName, ItemID);    //추가


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAMPROJ_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnyWhere)					TArray<UItem*> m_AllItems;  //��� ������ ���

	UPROPERTY(EditAnyWhere)					UItem* m_EquippedItem;  //������ ������



public:
	UItemComponent();

	UPROPERTY(BlueprintAssignable)				FOnItemReady OnItemReady;
	UPROPERTY(BlueprintAssignable)				FOnItemQuantityChanged OnItemQuantityChanged;
	UPROPERTY(BlueprintAssignable)				FOnItemDataChanged OnItemDataChanged;  //추가

protected:
	virtual void BeginPlay() override;


public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void InitAllItems();  //��� ������ ��� �ʱ�ȭ

	UFUNCTION(BlueprintCallable)
	bool EquipItem(FName ItemID);  //����

	UFUNCTION(BlueprintCallable)
	bool UnequipItem();  //����

	UFUNCTION(BlueprintCallable)
	bool ChangeItem(FName ItemID);  //���� �� ����

	UFUNCTION(BlueprintCallable)
	bool TryUseItem(AActor* OptionalTarget = nullptr, FVector OptionalLocation = FVector::ZeroVector);  //������ ���

	UFUNCTION(BlueprintCallable)
	bool AddItemQuantity(FName ItemID, int32 Amount);  //���� ����

	UFUNCTION(BlueprintCallable)
	bool RemoveItemQuantity(FName ItemID, int32 Amount);  //���� ����

	UFUNCTION(BlueprintPure)
	UItem* GetEquippedItem() const;  //������ ������ UItem �����ͷ� ��ȯ

	UFUNCTION(BlueprintPure)
	FName GetEquippedItemID() const;  //������ ������ ItemID ��ȯ

	UFUNCTION(BlueprintPure)
	UItem* GetAllItem(FName ItemID) const;  //��� ������ ��Ͽ��� ItemID �˻��Ͽ� UItem �����ͷ� ��ȯ

	UFUNCTION(BlueprintPure)
	TArray<UItem*> GetAllItemData() const { return m_AllItems; }  //��� ������ ����� ��� UItem �����ͷ� ��ȯ

	UFUNCTION(BlueprintPure)
	int32 GetItemQuantity(FName ItemID) const;  //��� ������ ��Ͽ��� ItemID�� ���� ��ȯ

	UFUNCTION(BlueprintPure)
	int32 GetItemPrice(FName ItemID) const;  //��� ������ ��Ͽ��� ItemID�� ���� ��ȯ

	UFUNCTION(BlueprintPure)
	int32 GetItemSellPrice(FName ItemID) const;  //��� ������ ��Ͽ��� ItemID�� �ǸŰ� ��ȯ (���� ���� 50%, �ּ� 1��)

	UFUNCTION(BlueprintPure)
	bool HasEquippedItem() const;  //������ �������� nullptr���� üũ //true: ������ //false: nullptr

	UFUNCTION(BlueprintPure)
	bool HasQuantity(FName ItemID) const;  //������ 1 �̻����� üũ

	UFUNCTION(BlueprintCallable)
	void UpdateItemData(FName ItemID, const FItemData& NewItemData);  //추가
};
