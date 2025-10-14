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
	
private:
	AGold();

protected:
	virtual void OnGet() override;
};
