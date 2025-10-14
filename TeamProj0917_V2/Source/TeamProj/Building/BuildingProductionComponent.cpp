// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingProductionComponent.h"



UBuildingProductionComponent::UBuildingProductionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBuildingProductionComponent::InitBuildingEffect(const FBuildingLevelData& Row)
{
	CurrentLevel = Row.Level;
	Effects = Row.buildingEffects;
	
	InitChild();

	
}

// Called when the game starts
void UBuildingProductionComponent::BeginPlay()
{
	OnBinding();
}

bool UBuildingProductionComponent::CheckEffect(FBuildingEffect ef)
{
	for (EBuildingEffectType Oef : OwnEffect)
	{
		if (ef.Type == Oef)
			return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("This effect is not a member of effect : This : [%s]"),
		 *UEnum::GetValueAsString(ef.Type));

	return false;
}


// Called every frame
void UBuildingProductionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuildingProductionComponent::OnActivate(const FBuildingEffect& effect)
{
	if (effect.isGlobal)
		RegisterEffect(effect);
	else
		ActivateEffect(effect);
}

void UBuildingProductionComponent::OnDeActivate(const FBuildingEffect& effect)
{
	if (effect.isGlobal)
	{
		UnRegisterEffect(effect);
	}
	else
	{
		DeActivateEffect(effect);
	}
}

void UBuildingProductionComponent::OnExplosion()
{
	if (!bIsBomb)
		return;

	FVector pos = GetOwner()->GetActorLocation();

	//터지는거 구현.
}

