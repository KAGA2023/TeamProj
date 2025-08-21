// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillStruct.h"
#include "SkillComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAMPROJ_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	TArray<USkill*> m_Skills;
	int32 m_Size;
	bool bPlayer;

public:	
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void InitSkill(const TArray<FSkillConfig>& SkillSetConfig, int32 Level);

	UFUNCTION(BlueprintCallable)
	void ApplySkillEffect(USkill* pSkill, AActor* pTarget);

	UFUNCTION(BlueprintCallable)
	bool TryUseSKill(int32 Index);

	UFUNCTION(BlueprintCallable)
	bool UpgradeSkill(FName SkillID);

		
};


