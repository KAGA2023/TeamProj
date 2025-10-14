//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "BuildingBase.h"
//#include "BuildingProductionComponent.h"
//#include "MainCastle.generated.h"
//
//class AMyGamePlayerState;
//class AGameCharacterBase;
//class UStatusComponent;
//
///**
// * 
// */
//
//
//UCLASS()
//class TEAMPROJ_API AMainCastle : public ABuildingBase
//{
//	GENERATED_BODY()
//
//public:
//	AMainCastle();
//
//protected:
//	// 레벨업 시 특성 선택 UI를 자동 호출할지
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main")
//	bool bOpenTraitUIOnLevelUp = true;
//
//	// 레벨 1에서는 특성창을 열지 않도록 하기 위함
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
//	bool bFirstApplyDone = false;
//
//public:
//	// 블루프린트에서 바인딩해서 UI 열기 등 처리 가능
//	UPROPERTY(BlueprintAssignable, Category = "Main")
//	FOnTraitSelectionRequested OnTraitSelectionRequested;
//
//	// 핵심: 공통 적용(Super) + 메인성 전용 효과 처리
//	virtual void ApplyRow(const FBuildingRow& Row) override;
//protected:
//
//	// PlayerState 헬퍼
//	AMyGamePlayerState* GetTPPlayerState() const;
//
//	// 효과 처리
//	void HandleAddReRoll(int32 Delta);
//	void HandleMatterLimitAdd(int32 Delta);
//	//특성 핸들 ++
//};
