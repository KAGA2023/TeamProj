// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "SkillTrigger.h"
#include "../Damage/DamageComponent.h"
#include "../Status/StatusComponent.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_Skills.Empty();
	m_Size = 4;



}


void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	InitSlots(m_Size);
	
} 


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(int32 i = 0; i < m_Skills.Num(); ++i)
	{
		if (USkill* Skill = m_Skills[i])
		{
			const bool IsReadySkill = Skill->IsReady();
			Skill->CoolDown(DeltaTime);
			if (Skill->IsReady() && !IsReadySkill)
				OnSkillReady.Broadcast(i);
		}
	}


}

void USkillComponent::InitSlots(int32 MaxSlots)
{
	m_Size = FMath::Max(1, MaxSlots);
	m_Skills.SetNum(m_Size);
}



//�÷��̾�� ������ bIsPlqyer = true
void USkillComponent::InitSkill(const TArray<FSkillConfig>& SkillSetConfig, int32 Level)
{

	UDataTable* SkillDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Skill/DT_Skill.DT_Skill"));
	if (!SkillDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Load SkillDataTable"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("SkillDataTable Load Success"));
	TArray<FSkillData*> AllRows;
	SkillDataTable->GetAllRows(TEXT("SkillInit"), AllRows);

	int32 CurrentIndex = 0;

	for (const FSkillConfig& Config : SkillSetConfig)
	{
		const FString TargetClass = Config.ClassType.ToString(); //"Player","Sword" , "02" -> �÷��̾�Ŭ���� �˹���Ÿ���� 2����ų
		const FString WeaponType = Config.WeaponType.ToString();
		int32 SkillCount = Config.Count; //1

		int32 FoundCount = 0;

		for (FSkillData* Row : AllRows) //1~3
		{
			if (!Row) continue;

			if (!Row->SkillID.ToString().StartsWith(TargetClass)) continue;
			if (!Row->WeaponType.ToString().StartsWith(WeaponType)) continue;
			if (FoundCount >= SkillCount) break; // 0>= 1

			USkill* NewSkill = NewObject<USkill>(this);
			if (!NewSkill) return;
			NewSkill->SetSkillData(*Row);
			NewSkill->SetSkillLevel(Level);

			m_Skills[CurrentIndex++] = NewSkill;
			FoundCount++;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("SkillInit Load Success"));

}


bool USkillComponent::TryUseSKill(int32 Index, AActor* OptionalTarget, FVector OptionalLocation)
{
	if (!m_Skills.IsValidIndex(Index)) return false;

	if (isSkillCasting) return false;

	if (AGameCharacterBase* Owner = Cast<AGameCharacterBase>(GetOwner()))
	{
		if (UStatusComponent* StatComp = Owner->FindComponentByClass<UStatusComponent>())
			if (StatComp->IsDead())
				return false;
	}

	bool SkillUse = false;
	if (USkill* Skill = m_Skills[Index])
	{
		if (Skill->GetSkillData().TriggerClass != nullptr)
		{
			
			SkillUse =  Skill->ExecuteSkill(GetOwner(), OptionalTarget, OptionalLocation);
		}
	}

	if (bUseNotifyFlow)
	{
		SkillUse =   BeginCast(Index, OptionalTarget, OptionalLocation);
	}
	else
	{
		if (USkill* Skill = m_Skills[Index])
		{
			SkillUse =  Skill->ExecuteSkill(GetOwner(), OptionalTarget, OptionalLocation);
		}
	}

	USkill* Skill = m_Skills[Index];
	if (!Skill) return false;
	if (Skill->IsReady())
	{
		isSkillCasting = true;
		return SkillUse;
	}
	return false;
}

bool USkillComponent::UpgradeSkill(FName SkillID)
{
	for (USkill* Slot : m_Skills)
		if (Slot && Slot->GetSkillData().SkillID == SkillID)
			return Slot->SkillLevelUp();
	return false;
}

USkill* USkillComponent::GetSkillSlot(int32 Index) const
{
	if (!m_Skills.IsValidIndex(Index)) return nullptr;
	return m_Skills[Index];
}

void USkillComponent::OnCastSkillStart(UAnimMontage* Montage, bool bInterrupted)
{

	isSkillCasting = false;
	UE_LOG(LogTemp, Warning, TEXT("TrySkill? = %s"), isSkillCasting ? TEXT("true") : TEXT("false"));
}

void USkillComponent::UpdateSkillData(FName SkillID, const FSkillData& NewSkillData)  //추가
{
	UE_LOG(LogTemp, Warning, TEXT("SkillComponent: UpdateSkillData 호출됨 - SkillID: %s"), *SkillID.ToString());
	
	// 해당 스킬 찾기
	for (USkill* Skill : m_Skills)
	{
		if (Skill && Skill->GetSkillData().SkillID == SkillID)
		{
			UE_LOG(LogTemp, Warning, TEXT("SkillComponent: 스킬 찾음 - SkillID: %s, SkillName: %s"), 
			       *SkillID.ToString(), *Skill->GetSkillData().SkillName);
			
			Skill->SetSkillData(NewSkillData);
			
			UE_LOG(LogTemp, Warning, TEXT("SkillComponent: 스킬 데이터 설정 완료 - SkillID: %s"), *SkillID.ToString());
			
			// 스킬 데이터 변경 알림
			UE_LOG(LogTemp, Warning, TEXT("SkillComponent: OnSkillDataChanged 델리게이트 브로드캐스트 시작 - SkillID: %s"), *SkillID.ToString());
			OnSkillDataChanged.Broadcast(SkillID);
			UE_LOG(LogTemp, Warning, TEXT("SkillComponent: OnSkillDataChanged 델리게이트 브로드캐스트 완료 - SkillID: %s"), *SkillID.ToString());
			
			UE_LOG(LogTemp, Log, TEXT("SkillComponent: Skill data updated for %s"), *SkillID.ToString());
			return;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("SkillComponent: Skill not found for ID %s"), *SkillID.ToString());
}

