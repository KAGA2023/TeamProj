// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBase.h"
#include "Barrack.generated.h"

UENUM()
enum class EWeoponClass : uint8
{
	Worrior,
	Archer,
	Priest,
	Wizard
};

UCLASS()
class TEAMPROJ_API ABarrack : public ABuildingBase
{
	GENERATED_BODY()

public:
	ABarrack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> SpawnPosList
	{
		FVector(-150.f,-225.f,0.f),FVector(-150.f,-75.f,0.f),
		FVector(-150.f,75.f,0.f),FVector(-150.f,225.f,0.f),
		FVector(-300.f,-225.f,0.f),FVector(-300.f,-75.f,0.f),
		FVector(-300.f,75.f,0.f),FVector(-300.f,225.f,0.f),
	};

private:
	void SetSpawnposList();

protected:
	virtual void PostApplyRow() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IdentityList")
	TArray<FName> IdentityList; // 생성자에서 초기화

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurIdentity")
	FName CurSpawnIdentity;	//생성자에서 초기화
	
	UFUNCTION()
	void SelectIdentity(FName select);
};
