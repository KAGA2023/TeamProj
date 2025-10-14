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

public:
	UPerkComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};