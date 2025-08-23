// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillStruct.h"
#include "../Character/GameCharacterBase.h"
#include "SkillTrigger.h"

USkill::USkill()
{
	m_SkillName = TEXT("");
	m_CurrentLevel = 0;
	m_CoolRemaining = 0.f;

}



const FSkillLevelData& USkill::GetSkillLevelData() const
{
	static FSkillLevelData SData;
	return m_SkillData.SkillLevels.IsValidIndex(m_CurrentLevel) ? m_SkillData.SkillLevels[m_CurrentLevel] : SData;
}

void USkill::SetSkillData(const FSkillData& pData)
{
	this->m_SkillData = pData;
	this->m_CoolRemaining = 0.f;
}

void USkill::SetSkillLevel(int32 nLevel)
{
	this->m_CurrentLevel = nLevel;
}

bool USkill::SkillLevelUp()
{
	if (m_SkillData.SkillLevels.IsValidIndex(m_CurrentLevel + 1))
	{
		m_CurrentLevel++;
		return true;
	}
	return false;
}

bool USkill::ExecuteSkill(AActor* Caster, AActor* OptionalTarget, const FVector& OptionalLocation)
{
	if (!IsReady() || !Caster) return false;
	FSkillContext Ctx;
	Ctx.Caster = Caster;
	Ctx.OptionalTarget = OptionalTarget;
	Ctx.OptionalLocation = OptionalLocation;
	Ctx.SkillLevel = m_CurrentLevel;
	Ctx.Data = &m_SkillData;

	USkillTriggerStrategy* Trigger = m_SkillData.TriggerClass ? NewObject<USkillTriggerStrategy>(this, m_SkillData.TriggerClass) : nullptr;
	if (!Trigger) return false;

	//트리거 실행 (발사 타이밍에 OnFire() ㄱㄱ
	FOnFireWithCtx Delegate = FOnFireWithCtx::CreateLambda([this](const FSkillContext& TriggeredCtx)
	{
			//타게팅 ㄱㄱ213123
			TArray<AActor*> Targets;
			ETargetAffiliation UsedAffiliation = m_SkillData.TargetAffiliation; 

			if (TriggeredCtx.Data && TriggeredCtx.Data->TargetingClass)
			{

				if (UTargetingTask* Tgt = NewObject<UTargetingTask>(this, m_SkillData.TargetingClass))
				{
					//UE_LOG(LogTemp, Warning, TEXT("TargetingTask = %s"), *Tgt->GetName());
					Tgt->Affiliation = m_SkillData.TargetAffiliation;
					Tgt->ResolveTargets(TriggeredCtx, Targets);

					UsedAffiliation = Tgt->Affiliation;//자식내에서 바뀔경우?
				}
			}

			if (Targets.Num() == 0 && UsedAffiliation == ETargetAffiliation::SelfOnly && IsValid(TriggeredCtx.Caster)) // 자기자신일때 타게팅이 비워질경우대비 자기자신 추가
			{
				Targets.AddUnique(TriggeredCtx.Caster);
			}


			//효과 CC,상태이상
			for (auto Efs : m_SkillData.Effects) //흠.......
			{
				if (!Efs) continue;

				if (USkillEffect* Effect = NewObject<USkillEffect>(this, Efs))
				{
					//UE_LOG(LogTemp, Warning, TEXT("TargetingTask = %s"), *Effect->GetName());
					for (AActor* T : Targets)
					{
						if (!IsValid(T)) continue;

					    if (!TeamFilterUtil::PassAffiliation(TriggeredCtx.Caster,T, m_SkillData.TargetAffiliation)) continue;

						Effect->Apply(TriggeredCtx, T);
					}
				}
			}

			CoolStart();
	});

	Trigger->ExecuteTrigger(Ctx, &Delegate);

	//이팩트 재생? 사운드 재생?
	//TriggerStrategy에 애님노티파이에서 발사, 캐스팅 바 끝나면 발사, 발사체 히트시점에 발사 같은 변형도 가능하ㄹ듯??

	return true;
}

bool USkill::CoolStart()
{
	if (m_SkillData.SkillLevels.IsValidIndex(m_CurrentLevel))
	{
		const float BaseCoolDown = m_SkillData.Cooldown - GetSkillLevelData().CoolRec; //캐릭터 스탯 추가가속까지 더해야함
		m_CoolRemaining = FMath::Max(BaseCoolDown, 0.f);
		return true;
	}
	return false;
}

void USkill::CoolDown(float DeltaTime)
{
	if (m_CoolRemaining > 0.f)
	{
		m_CoolRemaining -= DeltaTime;
		if (m_CoolRemaining < 0.f)
		{
			m_CoolRemaining = 0.f;
		}
	}
}
