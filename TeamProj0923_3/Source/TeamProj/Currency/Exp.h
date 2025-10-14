// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CurrencyBase.h"
#include "Exp.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API AExp : public ACurrencyBase
{
	GENERATED_BODY() 
	
protected:
	UPROPERTY(BlueprintReadOnly)
	int32 Exp{};
	
private:
	AExp();

protected:
	virtual void OnGet() override;

public:
	UFUNCTION(BlueprintCallable)
	void GetExp(int32 getExp);
};
