// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillTrigger.h"
#include "../Damage/DamageComponent.h"
#include "../Status/StatusComponent.h"
#include "../Character/GameCharacterBase.h"
#include "Effect.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API UEffect_Damage : public USkillEffect
{
	GENERATED_BODY()
	
public:
	virtual void Apply(const FSkillContext& Ctx, AActor* Target) override
	{
		if (!Ctx.Data || !Target) return;

		const float AddDmg = (Ctx.Data->SkillLevels.IsValidIndex(Ctx.SkillLevel) ? Ctx.Data->SkillLevels[Ctx.SkillLevel].AddDamage : 0.f); 

		if (UDamageComponent* DamageComp = Target->FindComponentByClass<UDamageComponent>())
		{
			AGameCharacterBase* pCaster = Cast<AGameCharacterBase>(Ctx.Caster);
			if (!pCaster) return;

			UStatusComponent* CasterStatComp = pCaster->FindComponentByClass<UStatusComponent>();
			if (!CasterStatComp) return;

			AGameCharacterBase* pTarget = Cast<AGameCharacterBase>(Target);
			if (!pTarget) return;

			UStatusComponent* TargetStatComp = pTarget->FindComponentByClass<UStatusComponent>();
			if (!TargetStatComp) return;

			float CritRValue = FMath::FRand();

			const bool bIsCritR = CritRValue < CasterStatComp->m_FinalStat.CritRate;
			
			const float BeforeCritDmg = CasterStatComp->m_FinalStat.Attack + AddDmg;

			float AfterCritDmg;
			if (bIsCritR) AfterCritDmg = (1.f + CasterStatComp->m_FinalStat.CritDamage) / 100.f * BeforeCritDmg;
			else          AfterCritDmg = BeforeCritDmg;
		
			float TargetDef = (100 - TargetStatComp->m_FinalStat.Defense) / 100;
			const float FinalDmg = AfterCritDmg*TargetDef;

			DamageComp->CalculateDamage(Target,FinalDmg); 
		}
	}
};


UCLASS()
class TEAMPROJ_API UEffect_AddStatus : public USkillEffect
{
	GENERATED_BODY()
	
public:
	virtual void Apply(const FSkillContext& Ctx, AActor* Target)  override 
	{
		if (UDamageComponent* DamageComp = Target->FindComponentByClass<UDamageComponent>())
		{
			const FSkillEffectInfo& Effect = Ctx.Data->SkillLevels[Ctx.SkillLevel].ExtraEffect;
			DamageComp->AddStatusEffect(Effect);
		}
	}
};