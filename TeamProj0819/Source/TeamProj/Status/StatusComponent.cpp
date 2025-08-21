// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_CurrentStat.CntLevel = 0;
	m_CurrentStat.CntHP = 0.f;
	m_CurrentStat.CntEXP = 0;
	// ...
}

void UStatusComponent::Init(const EClassType ClassType, const FClassConfig& pClassSetConfig)
{
	m_ClassType = ClassType;
	m_StatTable = nullptr;
	m_Level = 1;

	switch (m_ClassType)
	{
	case EClassType::None: 
		break;
	case EClassType::Player: 
		m_StatTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Status/DT_PlayerStat.DT_PlayerStat"));
		if (!m_StatTable) return;
		break;
	case EClassType::Ally:
		m_StatTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Status/DT_AllyStat.DT_AllyStat"));
		if (!m_StatTable) return;
		break;
	case EClassType::Enemy:
		m_StatTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Status/DT_EnemyStat.DT_EnemyStat"));
		if (!m_StatTable) return;
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Stat DataTable Load Succeed"));

	m_ClassID = pClassSetConfig.ClassType;
	m_WeaponID = pClassSetConfig.WeaponType;

	
	TArray<FBaseStat*> AllRows;
	m_StatTable->GetAllRows(TEXT("StatInit"), AllRows);

	FBaseStat* Found = nullptr;
	for (FBaseStat* Row : AllRows)
	{
		if (!Row) continue;
		
		if (Row->ClassID == m_ClassID && Row->WeaponID == m_WeaponID && Row->Level == pClassSetConfig.Level)
		{
			Found = Row;
			break;
		}
	}

	if (!Found)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stat init Failed: %s / %s / L%d"),
			*m_ClassID.ToString(), *m_WeaponID.ToString(), pClassSetConfig.Level);
		return;
	}

	m_BaseStat = *Found;
	m_WeaponStat = Found->WeaponStat;

	
	m_CurrentStat.CntLevel = Found->Level;
	m_CurrentStat.CntHP = Found->MaxHP;  
	AddModifier(m_WeaponStat); 
	ProgressHPMgr(0);
	UE_LOG(LogTemp, Warning, TEXT("Stat init Succeed"));
}


void UStatusComponent::InitFromBaseStat(const FBaseStat& InBase, EStatusOwnerType InOwnerType, bool bResetHP)
{
	OwnerType = InOwnerType;
	m_StatTable = nullptr;        // 캐릭터 테이블 미사용
	m_ClassType = EClassType::None;
	m_ClassID = NAME_None;
	m_WeaponID = NAME_None;
	m_Level = 1;

	m_BaseStat = InBase;
	m_WeaponStat = FStatModifier{};    // 건물은 무기 모디파이어 없음
	m_StatModifier.Reset();

	ReCalculationStat();

	m_CurrentStat.CntLevel = 1;
	if (bResetHP) m_CurrentStat.CntHP = m_FinalStat.MaxHP;
}

void UStatusComponent::SetBaseStat(const FBaseStat& InBase, bool bKeepHpPct)
{
	const float PrevMax = m_FinalStat.MaxHP;
	const float HpPct = (PrevMax > 0.f) ? (m_CurrentStat.CntHP / PrevMax) : 0.f;

	m_BaseStat = InBase;
	ReCalculationStat();

	if (bKeepHpPct)
		ProgressHPMgr(HpPct);
}

void UStatusComponent::TakeDamage(float DamageAmount)
{
	const float TotalDmg = DamageAmount * -1;
	ProgressHPMgr(TotalDmg);
	const bool bDead = (m_CurrentStat.CntHP <= 0.f);
	OnDeath(bDead);
	if (bDead)
	{
		if (AActor* Owner = GetOwner())
		{
			OnDied.Broadcast(Owner);
		}
	}
}

void UStatusComponent::StartHPProgress()
{
	m_bHPRotationRatio = true;
}


void UStatusComponent::ProgressHPMgr(float Amount)
{
	m_CurrentStat.CntHP = FMath::Clamp( m_CurrentStat.CntHP + Amount, 0.f, m_FinalStat.MaxHP);
	m_HPFrontRatio = m_CurrentStat.CntHP / m_FinalStat.MaxHP;

	if (m_HPFrontRatio >= m_HPBackRatio)
		m_HPBackRatio = m_HPFrontRatio;
	else
		GetOwner()->GetWorldTimerManager().SetTimer(Timer_StartHPProgress, this, &UStatusComponent::StartHPProgress, 1.f, false, 0.5f);
}





void UStatusComponent::AddModifier(const FStatModifier& pModi)
{
	m_StatModifier.Add(pModi);
	ReCalculationStat();
}

void UStatusComponent::RemoveModifier(const FStatModifier& pModi)
{
	m_StatModifier.Remove(pModi);
	ReCalculationStat();
}

void UStatusComponent::ReCalculationStat()
{
	FStatModifier TotalMod = {};

	for (const auto& Mod : m_StatModifier)
	{
		TotalMod.AddHP += Mod.AddHP;
		TotalMod.AddAtk += Mod.AddAtk;
		TotalMod.AddDef += Mod.AddDef;
		TotalMod.AddCritR += Mod.AddCritR;
		TotalMod.AddCritD += Mod.AddCritD;
		TotalMod.AddAtkSpd += Mod.AddAtkSpd;
		TotalMod.AddAtkRadius += Mod.AddAtkRadius;
		TotalMod.AddSpd += Mod.AddSpd;
		TotalMod.AddCoolRec += Mod.AddCoolRec;
	}

	m_FinalStat.MaxHP				= m_BaseStat.MaxHP				+ TotalMod.AddHP;
	m_FinalStat.Attack				= m_BaseStat.Attack				+ TotalMod.AddAtk;
	m_FinalStat.Defense				= m_BaseStat.Defense			+ TotalMod.AddDef;
	m_FinalStat.CritRate			= m_BaseStat.CritRate			+ TotalMod.AddCritR;
	m_FinalStat.CritDamage			= m_BaseStat.CritDamage			+ TotalMod.AddCritD;
	m_FinalStat.AttackSpeed			= m_BaseStat.AttackSpeed		+ TotalMod.AddAtkSpd;
	m_FinalStat.AttackRadius		= m_BaseStat.AttackRadius		+ TotalMod.AddAtkRadius;
	m_FinalStat.Speed				= m_BaseStat.Speed				+ TotalMod.AddSpd;
	m_FinalStat.CooldownReduction	= m_BaseStat.CooldownReduction	+ TotalMod.AddCoolRec;
	m_FinalStat.ReqEXPToNextLevel	= m_BaseStat.ReqEXPToNextLevel;
}


void UStatusComponent::AddExp(int32 ExpAmount)
{
	m_CurrentStat.CntEXP += ExpAmount;
	while (m_CurrentStat.CntEXP >= m_FinalStat.ReqEXPToNextLevel)
	{
		m_CurrentStat.CntEXP -= m_FinalStat.ReqEXPToNextLevel;
		LevelUp();
	}
}

void UStatusComponent::LevelUp()
{
	if (!m_StatTable) return;

	const int32 NewLevel = m_CurrentStat.CntLevel + 1;
	TArray<FBaseStat*> AllRows;
	m_StatTable->GetAllRows(TEXT("LevelUp"), AllRows);

	FBaseStat* Next = nullptr;
	for (FBaseStat* Row : AllRows)
	{
		if (!Row) continue;
		if (Row->ClassID == m_ClassID && Row->WeaponID == m_WeaponID && Row->Level == NewLevel)
		{
			Next = Row;
			break;
		}
	}

	if (!Next) return;

	const float PrevMax = m_FinalStat.MaxHP;
	m_BaseStat = *Next;
	ReCalculationStat();
	m_CurrentStat.CntLevel = NewLevel;
	const float LevelAddHP = m_FinalStat.MaxHP - PrevMax;

	ProgressHPMgr(LevelAddHP);

	UE_LOG(LogTemp, Log, TEXT("LevelUp -> L%d, MaxHP=%.1f, Attack=%.1f"),
	m_CurrentStat.CntLevel, m_FinalStat.MaxHP, m_FinalStat.Attack);

}

void UStatusComponent::OnDeath(bool isDead)
{
	//죽음이벤트
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_bHPRotationRatio)
	{
		m_HPBackRatio = m_HPBackRatio - (DeltaTime * 0.5f);
		if (m_HPBackRatio <= m_HPFrontRatio)
		{
			m_HPBackRatio = m_HPFrontRatio;
			m_bHPRotationRatio = false;
		}
	}
}

