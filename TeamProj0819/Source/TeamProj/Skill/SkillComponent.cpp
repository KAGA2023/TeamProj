// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "../Damage/DamageComponent.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Skills.Empty();
	m_Size = 0;
	bPlayer = false;

	
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (USkill* Skill : m_Skills)
	{
		if (!Skill) continue;

		bool bCoolDown = Skill->CoolDown(DeltaTime);

		if (bCoolDown)
		{
			//쿨 끝났을때 처리 onSkillCoolDownFinished.Broadcast(Slot);
		}
	}

}



//플레이어는 시작전 bIsPlqyer = true
void USkillComponent::InitSkill(const TArray<FSkillConfig>& SkillSetConfig, int32 Level)
{
	if (bPlayer) m_Skills.SetNum(4);
	else m_Skills.SetNum(3); //보스는 나중에 스위치 처리 ㄱㄱ

	UDataTable* SkillDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Skill/DT_Table.DT_Table"));
	if (!SkillDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Load SkillDataTable"));
		return;
	}

	TArray<FSkillData*> AllRows;
	SkillDataTable->GetAllRows(TEXT("SkillInit"), AllRows);

	int32 CurrentIndex = 0;

	for (const FSkillConfig& Config : SkillSetConfig)
	{
		const FString TargetClass = Config.ClassType.ToString(); //"Player","Sword" , "02" -> 플레이어클래스 검무기타입의 2번스킬
		const FString WeaponType = Config.WeaponType.ToString();
		int32 SkillCount = Config.Count;

		int32 FoundCount = 0;

		for (FSkillData* Row : AllRows)
		{
			if (!Row) continue;

			if (!Row->SkillID.ToString().StartsWith(TargetClass)) continue;
			if (!Row->WeaponType.ToString().StartsWith(WeaponType)) continue;
			if (FoundCount >= SkillCount) break;

			USkill* NewSkill = NewObject<USkill>(this);
			NewSkill->SetSkillData(*Row);
			NewSkill->SetSkillLevel(Level);

			m_Skills[CurrentIndex++] = NewSkill;

			FoundCount++;
		}
	}

}

void USkillComponent::ApplySkillEffect(USkill* pSkill, AActor* pTarget)
{
	if (!pSkill) return;

	const FSkillData& SkillData = pSkill->GetSkillData();
	const FSkillLevelData SkillLevelData = pSkill->GetSkillLevelData();

	TArray<AActor*> Targets;
	UDamageComponent* DmgComp = GetOwner()->FindComponentByClass<UDamageComponent>();
	if (!DmgComp) return;

	bool bCasterIsPlayer = GetOwner()->ActorHasTag("Player"); //플레이어 캐릭터 블루프린트 태그 설정해야함
	FSkillTraceParams pTrace;
	pTrace.Range = SkillData.Range + SkillLevelData.AddRange;
	pTrace.HalfHeight = SkillData.HalfHeight + SkillLevelData.AddHalfHeight;
	pTrace.Radius = SkillData.Radius + SkillLevelData.AddRadius;
	pTrace.ConeHalfAngle = 
	pTrace.Channel = ECC_Visibility;

	switch (SkillData.SkillTrigger)
	{
	case ESkillTriggerType::None: //대미지 트리거  DamageComponent->CollectTarget_Self(pTrace, Targets); break;
		break;
	case ESkillTriggerType::One:
		break;
	case ESkillTriggerType::SelfRadius:
		break;
	case ESkillTriggerType::TargetRadius:
		break;
	case ESkillTriggerType::TargetPoint:
		break;
	}

	for (AActor* Tgt : Targets)
	{
		if (!Tgt) continue;
		UDamageComponent* TgtDmgComp = Tgt->FindComponentByClass<UDamageComponent>();

		//TgtDmgComp->CalcuStatuEffect(SkillLevelData);
		//TgtDmgComp->CalcuCC(SkillLevelData);

		switch (SkillData.SkillType)
		{
		case ESkillType::None:
			break;
		case ESkillType::Attack: //TgtDamageComponent->CalculateDamage(SkillLevelData);
			break;
		case ESkillType::Buff:
			break;
		case ESkillType::DeBuff:
			break;
		case ESkillType::Heal:
			break;
		default:
			break;
		}
	}

}

bool USkillComponent::TryUseSKill(int32 Index)
{
	if (!m_Skills.IsValidIndex(Index)) return false;

	USkill* Skill = m_Skills[Index];
	if (!Skill || !Skill->IsReady()) return false;


	Skill->ExecuteSkill(GetOwner());
	//범위지정시?  플레이어 : 인터페이스 호출 -> (위치값) 
	//아군,적군 : 인터페이스 호출(타겟?)
	return true;
}

bool USkillComponent::UpgradeSkill(FName SkillID)
{
	for (USkill* Slot : m_Skills)
		if (Slot && Slot->GetSkillData().SkillID == SkillID)
			return Slot->SkillLevelUp();
	return false;
}


/*
//애니메이션에 노티파이 즈어교옹하므언 되앨드으읏
void UAnimNotify_DoMeleeHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Anim)
{
	if (AActor* Owner = MeshComp->GetOwner())
		if (USkillComponent* SC = Owner->FindComponentByClass<USkillComponent>())
		{
			// 현재 캐스팅 중인 슬롯/스킬 핸들로 ㄱㄱㄱ
			SC->ApplySkillEffect(CurrentCastingSlot, CurrentTarget);
		}
		이런식
}
*/