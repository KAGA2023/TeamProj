// Fill out your copyright notice in the Description page of Project Settings.

#include "PerkComponent.h"
#include "../Status/StatusComponent.h"

UPerkComponent::UPerkComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_AllPerks.Empty();
	m_ActivePerks.Empty();
}

void UPerkComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPerkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPerkComponent::AddPerk(UPerk* NewPerk)
{
	if (NewPerk && !m_AllPerks.Contains(NewPerk))
	{
		m_AllPerks.Add(NewPerk);
		UE_LOG(LogTemp, Log, TEXT("PerkComponent: Added perk %s"), *NewPerk->GetPerkName());
	}
}

void UPerkComponent::RemovePerk(UPerk* PerkToRemove)
{
	if (PerkToRemove && m_AllPerks.Contains(PerkToRemove))
	{
		// 퍼크가 활성화되어 있다면 먼저 비활성화
		if (PerkToRemove->IsActive())
		{
			PerkToRemove->DeactivatePerk();
		}
		
		m_AllPerks.Remove(PerkToRemove);
		UE_LOG(LogTemp, Log, TEXT("PerkComponent: Removed perk %s"), *PerkToRemove->GetPerkName());
	}
}

void UPerkComponent::ActivatePerkByID(const FName& PerkID)
{
	UPerk* Perk = FindPerkByID(PerkID);
	if (Perk)
	{
		Perk->ActivatePerk();
		
		// 활성화된 퍼크 목록에 추가 (중복 방지)
		if (!m_ActivePerks.Contains(Perk))
		{
			m_ActivePerks.Add(Perk);
		}
		
		UE_LOG(LogTemp, Log, TEXT("PerkComponent: Activated perk with ID %s"), *PerkID.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PerkComponent: Perk with ID %s not found"), *PerkID.ToString());
	}
}

void UPerkComponent::DeactivatePerkByID(const FName& PerkID)
{
	UPerk* Perk = FindPerkByID(PerkID);
	if (Perk)
	{
		Perk->DeactivatePerk();
		
		// 활성화된 퍼크 목록에서 제거
		m_ActivePerks.Remove(Perk);
		
		UE_LOG(LogTemp, Log, TEXT("PerkComponent: Deactivated perk with ID %s"), *PerkID.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PerkComponent: Perk with ID %s not found"), *PerkID.ToString());
	}
}

UPerk* UPerkComponent::FindPerkByID(const FName& PerkID)
{
	for (UPerk* Perk : m_AllPerks)
	{
		if (Perk && Perk->GetPerkData().PerkID == PerkID)
		{
			return Perk;
		}
	}
	return nullptr;
}
void UPerkComponent::ApplyActiveStatPerksToNewAlly(UStatusComponent* AllyStatusComp)
{
	if (!AllyStatusComp) return;
	
	int32 AppliedCount = 0;
	
	// 모든 활성화된 퍼크를 확인
	for (UPerk* ActivePerk : m_ActivePerks)
	{
		if (!ActivePerk) continue;
		
		// Stat 타입 퍼크인지 확인
		if (ActivePerk->GetPerkData().PerkType == EPerkType::Stat)
		{
			// UStatPerk로 캐스팅
			if (UStatPerk* StatPerk = Cast<UStatPerk>(ActivePerk))
			{
				// StatEffectType이 AllyStat인지 확인
				if (StatPerk->GetPerkData().StatEffectType == EStatEffectType::AllyStat)
				{
					// UAllyStatEffect로 캐스팅하여 캐시 시스템 활용
					if (UAllyStatEffect* AllyStatEffect = Cast<UAllyStatEffect>(StatPerk->CreateStatEffectStrategy(EStatEffectType::AllyStat)))
					{
						// 캐시된 효과를 새 아군에게 적용 (성능 최적화)
						AllyStatEffect->ApplyToNewAlly(AllyStatusComp);
						AppliedCount++;
						
						UE_LOG(LogTemp, Log, TEXT("PerkComponent: Applied cached stat perk %s to new ally"), 
							   *StatPerk->GetPerkData().PerkName);
					}
				}
			}
		}
	}
	
	if (AppliedCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("PerkComponent: Applied %d cached stat perks to new ally"), AppliedCount);
	}
}

void UPerkComponent::NotifyAllySpawned(UStatusComponent* NewAllyStatusComp)
{
	if (!NewAllyStatusComp) return;
	
	// 새로 소환된 아군에게 모든 활성화된 스탯 퍼크 적용
	ApplyActiveStatPerksToNewAlly(NewAllyStatusComp);
	
	UE_LOG(LogTemp, Log, TEXT("PerkComponent: Notified of new ally spawn, applied active stat perks"));
}

void UPerkComponent::AddAppliedAllyStatModifier(const FStatModifier& Modifier)
{
	m_AppliedAllyStatModifiers.Add(Modifier);
	UE_LOG(LogTemp, Log, TEXT("PerkComponent: Added applied ally stat modifier"));
}

void UPerkComponent::RemoveAppliedAllyStatModifier(const FStatModifier& Modifier)
{
	m_AppliedAllyStatModifiers.Remove(Modifier);
	UE_LOG(LogTemp, Log, TEXT("PerkComponent: Removed applied ally stat modifier"));
}

void UPerkComponent::ApplyAllAllyStatModifiersToNewAlly(UStatusComponent* NewAllyStatusComp)
{
	if (!NewAllyStatusComp) return;
	
	// 아군인지 확인
	if (NewAllyStatusComp->GetClassType() != EClassType::Ally) return;
	
	// 모든 적용된 모디파이어 적용
	for (const FStatModifier& Modifier : m_AppliedAllyStatModifiers)
	{
		NewAllyStatusComp->AddModifier(Modifier);
	}
	
	UE_LOG(LogTemp, Log, TEXT("PerkComponent: Applied %d ally stat modifiers to new ally"), m_AppliedAllyStatModifiers.Num());
}

void UPerkComponent::AddAppliedTargetedUnitModifier(const FName& UnitID, const FStatModifier& Modifier)
{
	FTargetedUnitModifier TargetedModifier;
	TargetedModifier.UnitID = UnitID;
	TargetedModifier.Modifier = Modifier;
	
	m_AppliedTargetedUnitModifiers.Add(TargetedModifier);
	UE_LOG(LogTemp, Log, TEXT("PerkComponent: Added targeted unit modifier for unit %s"), *UnitID.ToString());
}

void UPerkComponent::RemoveAppliedTargetedUnitModifier(const FName& UnitID, const FStatModifier& Modifier)
{
	FTargetedUnitModifier TargetedModifier;
	TargetedModifier.UnitID = UnitID;
	TargetedModifier.Modifier = Modifier;
	
	m_AppliedTargetedUnitModifiers.Remove(TargetedModifier);
	UE_LOG(LogTemp, Log, TEXT("PerkComponent: Removed targeted unit modifier for unit %s"), *UnitID.ToString());
}

void UPerkComponent::ApplyTargetedUnitModifiersToNewAlly(UStatusComponent* NewAllyStatusComp, const FName& UnitID)
{
	if (!NewAllyStatusComp) return;
	
	// 아군인지 확인
	if (NewAllyStatusComp->GetClassType() != EClassType::Ally) return;
	
	int32 AppliedCount = 0;
	
	// 해당 유닛 ID에 맞는 모디파이어들 적용
	for (const FTargetedUnitModifier& TargetedModifier : m_AppliedTargetedUnitModifiers)
	{
		if (TargetedModifier.UnitID == UnitID)
		{
			NewAllyStatusComp->AddModifier(TargetedModifier.Modifier);
			AppliedCount++;
		}
	}
	
	if (AppliedCount > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("PerkComponent: Applied %d targeted modifiers to unit %s"), AppliedCount, *UnitID.ToString());
	}
}
