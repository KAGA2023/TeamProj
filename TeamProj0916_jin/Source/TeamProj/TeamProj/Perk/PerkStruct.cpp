// Fill out your copyright notice in the Description page of Project Settings.

#include "PerkStruct.h"

UPerk::UPerk()
{
    m_PerkName = TEXT("");
    m_bIsActive = false;
}

void UPerk::SetPerkData(const FPerkData& pData)
{
    this->m_PerkData = pData;
    this->m_PerkName = pData.PerkName;
}

void UPerk::ActivatePerk()
{
    if (!m_bIsActive)
    {
        m_bIsActive = true;
        OnPerkActivated();
    }
}

void UPerk::DeactivatePerk()
{
    if (m_bIsActive)
    {
        m_bIsActive = false;
        OnPerkDeactivated();
    }
}

void UPerk::TogglePerk()
{
    if (m_bIsActive)
    {
        DeactivatePerk();
    }
    else
    {
        ActivatePerk();
    }
}

// 기본 구현 (자식 클래스에서 오버라이드)
void UPerk::OnPerkActivated_Implementation()
{
    // 기본 구현은 비어있음
}

void UPerk::OnPerkDeactivated_Implementation()
{
    // 기본 구현은 비어있음
}

// USummonPerk 구현
USummonPerk::USummonPerk()
{
}

void USummonPerk::OnPerkActivated_Implementation()
{
    // TODO: 실제 소환 로직 구현
    UE_LOG(LogTemp, Log, TEXT("SummonPerk: Activated"));
}

void USummonPerk::OnPerkDeactivated_Implementation()
{
    // TODO: 실제 소환 해제 로직 구현
    UE_LOG(LogTemp, Log, TEXT("SummonPerk: Deactivated"));
}

// UStatPerk 구현
UStatPerk::UStatPerk()
{
}

void UStatPerk::OnPerkActivated_Implementation()
{
    // TODO: 실제 스텟 적용 로직 구현
    UE_LOG(LogTemp, Log, TEXT("StatPerk: Activated"));
}

void UStatPerk::OnPerkDeactivated_Implementation()
{
    // TODO: 실제 스텟 제거 로직 구현
    UE_LOG(LogTemp, Log, TEXT("StatPerk: Deactivated"));
}

// UBuffPerk 구현
UBuffPerk::UBuffPerk()
{
}

void UBuffPerk::OnPerkActivated_Implementation()
{
    // TODO: 실제 버프 적용 로직 구현
    UE_LOG(LogTemp, Log, TEXT("BuffPerk: Activated"));
}

void UBuffPerk::OnPerkDeactivated_Implementation()
{
    // TODO: 실제 버프 제거 로직 구현
    UE_LOG(LogTemp, Log, TEXT("BuffPerk: Deactivated"));
}

// UItemPerk 구현
UItemPerk::UItemPerk()
{
}

void UItemPerk::OnPerkActivated_Implementation()
{
    // TODO: 실제 아이템 효과 적용 로직 구현
    UE_LOG(LogTemp, Log, TEXT("ItemPerk: Activated"));
}

void UItemPerk::OnPerkDeactivated_Implementation()
{
    // TODO: 실제 아이템 효과 제거 로직 구현
    UE_LOG(LogTemp, Log, TEXT("ItemPerk: Deactivated"));
}
