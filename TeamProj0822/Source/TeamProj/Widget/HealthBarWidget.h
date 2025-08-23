// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "HealthBarWidget.generated.h"

class AGameCharacterBase;
/**
 * 
 */
UCLASS()
class TEAMPROJ_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* m_CCType=nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* m_StatusEffectIcon= nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* m_ProgressBack=nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* m_ProgressFront = nullptr;

protected:
	UPROPERTY(BlueprintReadOnly)
	AGameCharacterBase* Class = nullptr;

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void Init(AGameCharacterBase* pClass);

	
};
