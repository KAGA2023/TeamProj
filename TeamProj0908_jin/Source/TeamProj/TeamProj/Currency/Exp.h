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
	
private:
	AExp();

protected:
	virtual void OnGet() override;
};
