// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CurrencyBase.h"
#include "Gold.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API AGold : public ACurrencyBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	int32 Money{};
	UPROPERTY(BlueprintReadOnly)
	UStaticMesh* money1{};
	UPROPERTY(BlueprintReadOnly)
	UStaticMesh* money2{};
	UPROPERTY(BlueprintReadOnly)
	UStaticMesh* money3{};
	UPROPERTY(BlueprintReadOnly)
	UStaticMesh* money4{};


	
private:
	AGold();

protected:
	virtual void OnGet() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void GetMoney(int32 getMoney);

};
