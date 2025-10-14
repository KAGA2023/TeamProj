// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerkStruct.h"
#include "PerkComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAMPROJ_API UPerkComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// 모든 퍼크 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perk")
	TArray<UPerk*> m_AllPerks;

	// 활성화된 퍼크들만 따로 추적 (새로 소환되는 아군에게 적용용)
	UPROPERTY()
	TArray<UPerk*> m_ActivePerks;

	// 현재 적용된 아군 스탯 모디파이어들 (새로 소환되는 아군에게 적용용)
	UPROPERTY()
	TArray<FStatModifier> m_AppliedAllyStatModifiers;

	// 특정 유닛 타겟팅 모디파이어들 (새로 소환되는 특정 유닛에게 적용용)
	UPROPERTY()
	TArray<FTargetedUnitModifier> m_AppliedTargetedUnitModifiers;

public:
	UPerkComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 퍼크 관리 함수들
	UFUNCTION(BlueprintCallable)
	void AddPerk(UPerk* NewPerk);

	UFUNCTION(BlueprintCallable)
	void RemovePerk(UPerk* PerkToRemove);

	UFUNCTION(BlueprintCallable)
	void ActivatePerkByID(const FName& PerkID);

	UFUNCTION(BlueprintCallable)
	void DeactivatePerkByID(const FName& PerkID);

	UFUNCTION(BlueprintCallable)
	UPerk* FindPerkByID(const FName& PerkID);

	UFUNCTION(BlueprintCallable)
	TArray<UPerk*> GetAllPerks() const { return m_AllPerks; }

	// 활성화된 퍼크 목록 반환
	UFUNCTION(BlueprintCallable)
	TArray<UPerk*> GetActivePerks() const { return m_ActivePerks; }

	// 새로 소환된 아군에게 모든 활성화된 스탯 퍼크 적용
	UFUNCTION(BlueprintCallable)
	void ApplyActiveStatPerksToNewAlly(class UStatusComponent* AllyStatusComp);

	// 아군 소환 시 호출되는 함수 (외부에서 호출)
	UFUNCTION(BlueprintCallable)
	void NotifyAllySpawned(class UStatusComponent* NewAllyStatusComp);

	// 적용된 아군 스탯 모디파이어 관리
	UFUNCTION(BlueprintCallable)
	void AddAppliedAllyStatModifier(const FStatModifier& Modifier);

	UFUNCTION(BlueprintCallable)
	void RemoveAppliedAllyStatModifier(const FStatModifier& Modifier);

	UFUNCTION(BlueprintCallable)
	void ApplyAllAllyStatModifiersToNewAlly(class UStatusComponent* NewAllyStatusComp);

	// 특정 유닛 타겟팅 모디파이어 관리
	UFUNCTION(BlueprintCallable)
	void AddAppliedTargetedUnitModifier(const FName& UnitID, const FStatModifier& Modifier);

	UFUNCTION(BlueprintCallable)
	void RemoveAppliedTargetedUnitModifier(const FName& UnitID, const FStatModifier& Modifier);

	UFUNCTION(BlueprintCallable)
	void ApplyTargetedUnitModifiersToNewAlly(class UStatusComponent* NewAllyStatusComp, const FName& UnitID);
};