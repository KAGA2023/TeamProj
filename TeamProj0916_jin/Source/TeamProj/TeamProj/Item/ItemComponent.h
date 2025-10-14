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
	UPROPERTY(EditAnyWhere)					TArray<UItem*> m_AllItems;  //���������̺�

	UPROPERTY(EditAnyWhere)					UItem* m_EquippedItem;  //������ ������

	

public:
	UItemComponent();

	UPROPERTY(BlueprintAssignable)				FOnItemReady OnItemReady;  //������ �غ񿩺� ��������Ʈ  //����
	UPROPERTY(BlueprintAssignable)				FOnItemQuantityChanged OnItemQuantityChanged;  //������ ���� ��ȭ ��������Ʈ  //����

protected:
	virtual void BeginPlay() override;


public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void InitAllItems();  //���������̺� �ʱ�ȭ

	UFUNCTION(BlueprintCallable)
	bool EquipItem(FName ItemID);  //����

	UFUNCTION(BlueprintCallable)
	bool UnequipItem();  //Ż��

	UFUNCTION(BlueprintCallable)
	bool ChangeItem(FName ItemID);  //Ż�� �� ����

	UFUNCTION(BlueprintCallable)
	bool TryUseItem(AActor* OptionalTarget = nullptr, FVector OptionalLocation = FVector::ZeroVector);  //������ ���

	UFUNCTION(BlueprintCallable)
	bool AddItemQuantity(FName ItemID, int32 Amount);  //����Up

	UFUNCTION(BlueprintCallable)
	bool RemoveItemQuantity(FName ItemID, int32 Amount);  //����Down

	UFUNCTION(BlueprintPure)
	UItem* GetEquippedItem() const;  //������ ������ UItem�������� ��ȯ

	UFUNCTION(BlueprintPure)
	FName GetEquippedItemID() const;  //������ ������ ItemID��ȯ

	UFUNCTION(BlueprintPure)
	UItem* GetAllItem(FName ItemID) const;  //���������̺����� ItemID ������ UItem�������� ��ȯ

	UFUNCTION(BlueprintPure)
	TArray<UItem*> GetAllItemData() const { return m_AllItems; }  //���������̺����� ���� UItem�������� ��ȯ

	UFUNCTION(BlueprintPure)
	int32 GetItemQuantity(FName ItemID) const;  //���������̺����� ItemID�� ���� ��ȯ

	UFUNCTION(BlueprintPure)
	int32 GetItemPrice(FName ItemID) const;  //���������̺����� ItemID�� ���� ��ȯ

	UFUNCTION(BlueprintPure)
	bool HasEquippedItem() const;  //���� ������ �������� nullptr���� üũ //true: �����Ȱ� ����  //false: nullptr

	UFUNCTION(BlueprintPure)
	bool HasQuantity(FName ItemID) const;  //������ 1�̻����� üũ
};
