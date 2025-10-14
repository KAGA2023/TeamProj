// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "BuildingBase.generated.h"

class UStatusComponent;
//플레이스테이트
class AMyGamePlayerState;
class UBuildingProductionComponent;
struct FBuildingLevelData;


UCLASS(Abstract)
class TEAMPROJ_API ABuildingBase : public AActor
{
	GENERATED_BODY()

	public:
	ABuildingBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DefaultMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* Boxcollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStatusComponent* StatusComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBuildingProductionComponent* ProductionComp; // 건물효과담당

	// 에디터에서 지정(Soft 권장). Soft 없으면 UDataTable*로 두고 그대로 사용해도 OK.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSoftObjectPtr<UDataTable> DataTableAsset;

	// 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBuilt = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDestroyed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float overlapoffset;

protected:
	virtual void BeginPlay() override;

	const FBuildingLevelData* GetRow(int32 InLevel) const;

	UFUNCTION()
	void UpdateVisualForLevel(const FString& Ref);

	UFUNCTION()
	virtual void OnBuildingDied(AActor* DeadOwner);
	UFUNCTION()
	void SetDataTable(FString path);

	UFUNCTION()
	void OnInteraction();

public:
	virtual void ApplyRow(const FBuildingLevelData& Row);

	UFUNCTION()
	void OnReceiveAnyDamage(float Amount);
	
	UFUNCTION()
	virtual void PostApplyRow() {};

	UFUNCTION()
	virtual void PreInit() {};

	UFUNCTION()
	virtual void PostInit() {};

	UFUNCTION(BlueprintCallable)
	void InitBuilding();

	UFUNCTION(BlueprintCallable)
	virtual void OpenUI();

	UFUNCTION(BlueprintCallable)
	bool LevelUp(); // 비용 체크는 외부(경제 시스템)

	UFUNCTION(BlueprintCallable)
	void Preview(bool isPressKey);

	UFUNCTION(BlueprintCallable)
	void ImportStageData(int32 lv); // 빌딩 스테이트가 사용.(데이터 넘겨주기.)

	UFUNCTION(BlueprintCallable)
	void SaveStageData(FString actRef, int32 lv, FVector pos, bool isbuilt);
};
