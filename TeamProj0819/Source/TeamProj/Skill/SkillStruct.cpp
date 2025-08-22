// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillStruct.h"
#include "../Character/GameCharacterBase.h"

USkill::USkill()
{
	m_SkillName = TEXT("");
	m_CurrentLevel = 0;
	m_Duration = 0.f;
	m_CoolRemaining = 0.f;
	m_CastingTime = 0.f;

}


FSkillLevelData USkill::GetSkillLevelData() const
{
	static FSkillLevelData EmptyLevelData;
	if (this->m_SkillData.SkillData.IsValidIndex(m_CurrentLevel))
		return m_SkillData.SkillData[m_CurrentLevel];
	return EmptyLevelData;
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
	if (m_SkillData.SkillData.IsValidIndex(m_CurrentLevel + 1))
	{
		m_CurrentLevel++;
		return true;
	}
	return false;
}

bool USkill::ExecuteSkill(AActor* Source)
{
	if (!IsReady() || !Source) return false;

	AGameCharacterBase* Caster = Cast<AGameCharacterBase>(Source);
	if (!Caster) return false;

	UAnimInstance* Anim = Caster->GetMesh() ? Caster->GetMesh()->GetAnimInstance() : nullptr;
	
	if (m_SkillData.Animation && Anim)
	{
		Anim->Montage_Play(m_SkillData.Animation);
	}

	if (m_SkillData.Sound)
	{
		//사운드 재생
	}
	
	//SkillComponent::UseSkill

	CoolStart();
	return true;
}

bool USkill::CoolStart()
{
	if (m_SkillData.SkillData.IsValidIndex(m_CurrentLevel))
	{
		float BaseCoolDown = m_SkillData.Cooldown - GetSkillLevelData().CoolRec; //캐릭터 스탯 추가가속까지 더해야함
		m_CoolRemaining = FMath::Max(BaseCoolDown, 0.f);
		return true;
	}
	return false;
}

bool USkill::CoolDown(float DeltaTime)
{
	if (m_CoolRemaining > 0.f)
	{
		m_CoolRemaining -= DeltaTime;
		if (m_CoolRemaining < 0.f)
		{
			m_CoolRemaining = 0.f;
			return true;
		}
	}
	return false;
}
