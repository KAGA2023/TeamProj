// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillStruct.h"
#include "SkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillReady, int32, SlotIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAMPROJ_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnyWhere)					TArray<USkill*> m_Skills;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	int32 m_Size;
	UPROPERTY(BlueprintAssignable)				FOnSkillReady OnSkillReady;

public:	
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void InitSlots(int32 MaxSlots);
	
	UFUNCTION(BlueprintCallable)
	void InitSkill(const TArray<FSkillConfig>& SkillSetConfig, int32 Level);

	UFUNCTION(BlueprintCallable)
	bool TryUseSKill(int32 Index, AActor* OptionalTarget = nullptr, FVector OptionalLocation = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable)
	bool UpgradeSkill(FName SkillID);
	
	UFUNCTION(BlueprintPure)
	USkill* GetSkillSlot(int32 Index) const;

		
};


