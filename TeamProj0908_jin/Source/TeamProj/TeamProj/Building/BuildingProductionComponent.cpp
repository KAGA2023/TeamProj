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
	//Super::BeginPlay();
	////건물고유효과 관리 -> 건물컴포넌트에서 
	//for (const FBuildingEffect& Eff : Row.Effects)
	//{
	//	switch (Eff.Type)
	//	{
	//	case EBuildingEffectType::AddStat:
	//		StatusComp->AddModifier(Eff.Stat);
	//		PrevAddStat = Eff.Stat;
	//		bHasPrevAddStat = true;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	// ...
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
		UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!"));
		UnRegisterEffect(effect);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("11111111"));
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

