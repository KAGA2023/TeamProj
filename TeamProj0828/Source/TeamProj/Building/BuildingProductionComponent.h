// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingProductionComponent.generated.h"

class ABuildingBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJ_API UBuildingProductionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	int32 Income = 0;



public:	
	// Sets default values for this component's properties
	UBuildingProductionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ClearBySource(ABuildingBase& Building);   //葛电积魂亲格力芭
	void RegisterIncome();
		
};
