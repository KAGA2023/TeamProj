// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingUI.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API ULoadingUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnLevelLoaded();
	float curPercent{}, targetPercent{};
	float DotTimer{};  // 점 깜빡임 타이머
	float BorderTimer{};  // 보더 회전 타이머

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Bar;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PercentText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* LoadingText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border1;  // 좌상
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border2;  // 우상
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border3;  // 우하
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border4;  // 좌하
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
public:
	virtual bool Initialize() override;
};
