// Fill out your copyright notice in the Description page of Project Settings.

#include "PerkStruct.h"
#include "../MyGamePlayerState.h"
#include "../Item/ItemComponent.h"
#include "../Status/StatusComponent.h"
#include "../Character/GameCharacterBase.h"
#include "PerkComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

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

// UItemEffectStrategy 기본 구현
bool UItemEffectStrategy::ApplyEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    return false;
}

bool UItemEffectStrategy::RemoveEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    return false;
}

// UGoldIncreaseEffect 구현
bool UGoldIncreaseEffect::ApplyEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World) return false;
    
    AMyGamePlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<AMyGamePlayerState>();
    if (!PlayerState) return false;
    
    int32 GoldIncrease = FMath::RoundToInt(PerkData.Value);
    PlayerState->Gold += GoldIncrease;
    
    UE_LOG(LogTemp, Log, TEXT("GoldIncreaseEffect: Gold increased by %d. Current Gold: %d"), 
           GoldIncrease, PlayerState->Gold);
    return true;
}

bool UGoldIncreaseEffect::RemoveEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World) return false;
    
    AMyGamePlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<AMyGamePlayerState>();
    if (!PlayerState) return false;
    
    int32 GoldDecrease = FMath::RoundToInt(PerkData.Value);
    PlayerState->Gold -= GoldDecrease;
    
    if (PlayerState->Gold < 0)
    {
        PlayerState->Gold = 0;
    }
    
    UE_LOG(LogTemp, Log, TEXT("GoldIncreaseEffect: Gold decreased by %d. Current Gold: %d"), 
           GoldDecrease, PlayerState->Gold);
    return true;
}

// UItemQuantityEffect 구현
bool UItemQuantityEffect::ApplyEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World || PerkData.ItemID == NAME_None) return false;
    
    AMyGamePlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<AMyGamePlayerState>();
    if (!PlayerState) return false;
    
    UItemComponent* ItemComponent = PlayerState->FindComponentByClass<UItemComponent>();
    if (!ItemComponent) return false;
    
    int32 QuantityIncrease = FMath::RoundToInt(PerkData.Value);
    bool bSuccess = ItemComponent->AddItemQuantity(PerkData.ItemID, QuantityIncrease);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("ItemQuantityEffect: Item %s quantity increased by %d"), 
               *PerkData.ItemID.ToString(), QuantityIncrease);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemQuantityEffect: Failed to add quantity for item %s"), 
               *PerkData.ItemID.ToString());
    }
    
    return bSuccess;
}

bool UItemQuantityEffect::RemoveEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World || PerkData.ItemID == NAME_None) return false;
    
    AMyGamePlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<AMyGamePlayerState>();
    if (!PlayerState) return false;
    
    UItemComponent* ItemComponent = PlayerState->FindComponentByClass<UItemComponent>();
    if (!ItemComponent) return false;
    
    int32 QuantityDecrease = FMath::RoundToInt(PerkData.Value);
    bool bSuccess = ItemComponent->RemoveItemQuantity(PerkData.ItemID, QuantityDecrease);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("ItemQuantityEffect: Item %s quantity decreased by %d"), 
               *PerkData.ItemID.ToString(), QuantityDecrease);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemQuantityEffect: Failed to remove quantity for item %s"), 
               *PerkData.ItemID.ToString());
    }
    
    return bSuccess;
}

// UStatEffectStrategy 기본 구현
bool UStatEffectStrategy::ApplyStatEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    return false;
}

bool UStatEffectStrategy::RemoveStatEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    return false;
}

// UPlayerStatEffect 구현
bool UPlayerStatEffect::ApplyStatEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World) return false;
    
    // 플레이어 캐릭터 찾기
    AMyGamePlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<AMyGamePlayerState>();
    if (!PlayerState) return false;
    
    // 플레이어 캐릭터의 StatusComponent 찾기
    UStatusComponent* StatusComponent = PlayerState->FindComponentByClass<UStatusComponent>();
    if (!StatusComponent) return false;
    
    // 스탯 모디파이어 생성
    FStatModifier StatModifier;
    StatModifier.AddHP = PerkData.HPBoost;
    StatModifier.AddAtk = PerkData.AttackBoost;
    StatModifier.AddDef = PerkData.DefenseBoost;
    StatModifier.AddSpd = PerkData.SpeedBoost;
    
    // 모디파이어 추가
    StatusComponent->AddModifier(StatModifier);
    
    UE_LOG(LogTemp, Log, TEXT("PlayerStatEffect: Applied HP=%.1f, Atk=%.1f, Def=%.1f, Spd=%.1f"), 
           PerkData.HPBoost, PerkData.AttackBoost, PerkData.DefenseBoost, PerkData.SpeedBoost);
    return true;
}

bool UPlayerStatEffect::RemoveStatEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World) return false;
    
    // 플레이어 캐릭터 찾기
    AMyGamePlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<AMyGamePlayerState>();
    if (!PlayerState) return false;
    
    // 플레이어 캐릭터의 StatusComponent 찾기
    UStatusComponent* StatusComponent = PlayerState->FindComponentByClass<UStatusComponent>();
    if (!StatusComponent) return false;
    
    // 스탯 모디파이어 생성 (제거용)
    FStatModifier StatModifier;
    StatModifier.AddHP = -PerkData.HPBoost;  // 음수로 제거
    StatModifier.AddAtk = -PerkData.AttackBoost;
    StatModifier.AddDef = -PerkData.DefenseBoost;
    StatModifier.AddSpd = -PerkData.SpeedBoost;
    
    // 모디파이어 제거
    StatusComponent->RemoveModifier(StatModifier);
    
    UE_LOG(LogTemp, Log, TEXT("PlayerStatEffect: Removed HP=%.1f, Atk=%.1f, Def=%.1f, Spd=%.1f"), 
           PerkData.HPBoost, PerkData.AttackBoost, PerkData.DefenseBoost, PerkData.SpeedBoost);
    return true;
}

// UAllyStatEffect 구현
bool UAllyStatEffect::ApplyStatEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World) return false;
    
    // 적용할 스탯 모디파이어 생성 및 저장
    m_AppliedModifier = FStatModifier();
    m_AppliedModifier.AddHP = PerkData.HPBoost;
    m_AppliedModifier.AddAtk = PerkData.AttackBoost;
    m_AppliedModifier.AddDef = PerkData.DefenseBoost;
    m_AppliedModifier.AddSpd = PerkData.SpeedBoost;
    
    // 기존 적용된 아군들 초기화
    m_AppliedAllies.Empty();
    
    // 첫 번째 적용 시에만 전체 검색 (성능 최적화)
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AGameCharacterBase::StaticClass(), FoundActors);
    
    int32 AppliedCount = 0;
    for (AActor* Actor : FoundActors)
    {
        AGameCharacterBase* Character = Cast<AGameCharacterBase>(Actor);
        if (!Character) continue;
        
        UStatusComponent* StatusComponent = Character->FindComponentByClass<UStatusComponent>();
        if (!StatusComponent) continue;
        
        // 아군인지 확인 (EClassType::Ally)
        if (StatusComponent->GetClassType() == EClassType::Ally)
        {
            // 특정 유닛 타겟팅인지 확인
            bool bShouldApply = true;
            if (PerkData.bTargetSpecificUnit && PerkData.TargetUnitID != NAME_None)
            {
                // 유닛 ID 확인 (StatusComponent에서 유닛 ID를 가져와야 함)
                // TODO: StatusComponent에 UnitID 필드 추가 필요
                // 현재는 모든 아군에게 적용 (임시)
                bShouldApply = true;
            }
            
            if (bShouldApply)
            {
                // 스탯 모디파이어 적용
                StatusComponent->AddModifier(m_AppliedModifier);
                
                // 적용된 아군을 캐시에 추가
                m_AppliedAllies.Add(StatusComponent);
                AppliedCount++;
            }
        }
    }
    
    // PerkComponent에 모디파이어 저장 (새로 스폰되는 아군에게 적용용)
    if (AMyGamePlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<AMyGamePlayerState>())
    {
        if (UPerkComponent* PerkComp = PlayerState->FindComponentByClass<UPerkComponent>())
        {
            // 특정 유닛 타겟팅인지 확인
            if (PerkData.bTargetSpecificUnit && PerkData.TargetUnitID != NAME_None)
            {
                PerkComp->AddAppliedTargetedUnitModifier(PerkData.TargetUnitID, m_AppliedModifier);
            }
            else
            {
                PerkComp->AddAppliedAllyStatModifier(m_AppliedModifier);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("AllyStatEffect: Applied to %d existing allies (cached for future spawns)"), AppliedCount);
    return AppliedCount > 0;
}

bool UAllyStatEffect::RemoveStatEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World) return false;
    
    // 캐시된 아군들에게만 효과 제거 (성능 최적화)
    int32 RemovedCount = 0;
    for (UStatusComponent* StatusComponent : m_AppliedAllies)
    {
        if (!StatusComponent) continue;
        
        // 스탯 모디파이어 제거 (음수로 제거)
        FStatModifier RemoveModifier;
        RemoveModifier.AddHP = -m_AppliedModifier.AddHP;
        RemoveModifier.AddAtk = -m_AppliedModifier.AddAtk;
        RemoveModifier.AddDef = -m_AppliedModifier.AddDef;
        RemoveModifier.AddSpd = -m_AppliedModifier.AddSpd;
        
        StatusComponent->RemoveModifier(RemoveModifier);
        RemovedCount++;
    }
    
    // PerkComponent에서 모디파이어 제거
    if (AMyGamePlayerState* PlayerState = World->GetFirstPlayerController()->GetPlayerState<AMyGamePlayerState>())
    {
        if (UPerkComponent* PerkComp = PlayerState->FindComponentByClass<UPerkComponent>())
        {
            // 특정 유닛 타겟팅인지 확인
            if (PerkData.bTargetSpecificUnit && PerkData.TargetUnitID != NAME_None)
            {
                PerkComp->RemoveAppliedTargetedUnitModifier(PerkData.TargetUnitID, m_AppliedModifier);
            }
            else
            {
                PerkComp->RemoveAppliedAllyStatModifier(m_AppliedModifier);
            }
        }
    }
    
    // 캐시 초기화
    m_AppliedAllies.Empty();
    m_AppliedModifier = FStatModifier();
    
    UE_LOG(LogTemp, Log, TEXT("AllyStatEffect: Removed from %d cached allies"), RemovedCount);
    return RemovedCount > 0;
}

void UAllyStatEffect::ApplyToNewAlly(UStatusComponent* NewAllyStatusComp)
{
    if (!NewAllyStatusComp) return;
    
    // 아군인지 확인
    if (NewAllyStatusComp->GetClassType() != EClassType::Ally) return;
    
    // 이미 적용된 아군인지 확인 (중복 방지)
    if (m_AppliedAllies.Contains(NewAllyStatusComp)) return;
    
    // 새 아군에게 스탯 모디파이어 적용
    NewAllyStatusComp->AddModifier(m_AppliedModifier);
    
    // 캐시에 추가
    m_AppliedAllies.Add(NewAllyStatusComp);
    
    UE_LOG(LogTemp, Log, TEXT("AllyStatEffect: Applied to new ally (total cached: %d)"), m_AppliedAllies.Num());
}

void UAllyStatEffect::RemoveFromAlly(UStatusComponent* AllyStatusComp)
{
    if (!AllyStatusComp) return;
    
    // 캐시에서 제거
    m_AppliedAllies.Remove(AllyStatusComp);
    
    // 스탯 모디파이어 제거
    FStatModifier RemoveModifier;
    RemoveModifier.AddHP = -m_AppliedModifier.AddHP;
    RemoveModifier.AddAtk = -m_AppliedModifier.AddAtk;
    RemoveModifier.AddDef = -m_AppliedModifier.AddDef;
    RemoveModifier.AddSpd = -m_AppliedModifier.AddSpd;
    
    AllyStatusComp->RemoveModifier(RemoveModifier);
    
    UE_LOG(LogTemp, Log, TEXT("AllyStatEffect: Removed from ally (remaining cached: %d)"), m_AppliedAllies.Num());
}

// UEnemyStatEffect 구현
bool UEnemyStatEffect::ApplyStatEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World) return false;
    
    // 모든 적군 캐릭터 찾기
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AGameCharacterBase::StaticClass(), FoundActors);
    
    int32 AppliedCount = 0;
    for (AActor* Actor : FoundActors)
    {
        AGameCharacterBase* Character = Cast<AGameCharacterBase>(Actor);
        if (!Character) continue;
        
        UStatusComponent* StatusComponent = Character->FindComponentByClass<UStatusComponent>();
        if (!StatusComponent) continue;
        
        // 적군인지 확인 (EClassType::Enemy)
        if (StatusComponent->GetClassType() == EClassType::Enemy)
        {
            // 스탯 모디파이어 생성
            FStatModifier StatModifier;
            StatModifier.AddHP = PerkData.HPBoost;
            StatModifier.AddAtk = PerkData.AttackBoost;
            StatModifier.AddDef = PerkData.DefenseBoost;
            StatModifier.AddSpd = PerkData.SpeedBoost;
            
            StatusComponent->AddModifier(StatModifier);
            AppliedCount++;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("EnemyStatEffect: Applied to %d enemies"), AppliedCount);
    return AppliedCount > 0;
}

bool UEnemyStatEffect::RemoveStatEffect_Implementation(const FPerkData& PerkData, UWorld* World)
{
    if (!World) return false;
    
    // 모든 적군 캐릭터 찾기
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AGameCharacterBase::StaticClass(), FoundActors);
    
    int32 RemovedCount = 0;
    for (AActor* Actor : FoundActors)
    {
        AGameCharacterBase* Character = Cast<AGameCharacterBase>(Actor);
        if (!Character) continue;
        
        UStatusComponent* StatusComponent = Character->FindComponentByClass<UStatusComponent>();
        if (!StatusComponent) continue;
        
        // 적군인지 확인 (EClassType::Enemy)
        if (StatusComponent->GetClassType() == EClassType::Enemy)
        {
            // 스탯 모디파이어 생성 (제거용)
            FStatModifier StatModifier;
            StatModifier.AddHP = -PerkData.HPBoost;
            StatModifier.AddAtk = -PerkData.AttackBoost;
            StatModifier.AddDef = -PerkData.DefenseBoost;
            StatModifier.AddSpd = -PerkData.SpeedBoost;
            
            StatusComponent->RemoveModifier(StatModifier);
            RemovedCount++;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("EnemyStatEffect: Removed from %d enemies"), RemovedCount);
    return RemovedCount > 0;
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
    if (m_PerkData.PerkType == EPerkType::Stat)
    {
        UWorld* World = GetWorld();
        if (!World) return;
        
        // 전략 패턴을 사용하여 스탯 효과 적용
        UStatEffectStrategy* StatStrategy = CreateStatEffectStrategy(m_PerkData.StatEffectType);
        if (StatStrategy)
        {
            StatStrategy->ApplyStatEffect(m_PerkData, World);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("StatPerk: No stat strategy found for type %d"), 
                   (int32)m_PerkData.StatEffectType);
        }
    }
}

void UStatPerk::OnPerkDeactivated_Implementation()
{
    if (m_PerkData.PerkType == EPerkType::Stat)
    {
        UWorld* World = GetWorld();
        if (!World) return;
        
        // 전략 패턴을 사용하여 스탯 효과 제거
        UStatEffectStrategy* StatStrategy = CreateStatEffectStrategy(m_PerkData.StatEffectType);
        if (StatStrategy)
        {
            StatStrategy->RemoveStatEffect(m_PerkData, World);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("StatPerk: No stat strategy found for type %d"), 
                   (int32)m_PerkData.StatEffectType);
        }
    }
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
    if (m_PerkData.PerkType == EPerkType::Item)
    {
        UWorld* World = GetWorld();
        if (!World) return;
        
        // 전략 패턴을 사용하여 효과 적용
        UItemEffectStrategy* EffectStrategy = CreateEffectStrategy(m_PerkData.ItemEffectType);
        if (EffectStrategy)
        {
            EffectStrategy->ApplyEffect(m_PerkData, World);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ItemPerk: No effect strategy found for type %d"), 
                   (int32)m_PerkData.ItemEffectType);
        }
    }
}

void UItemPerk::OnPerkDeactivated_Implementation()
{
    if (m_PerkData.PerkType == EPerkType::Item)
    {
        UWorld* World = GetWorld();
        if (!World) return;
        
        // 전략 패턴을 사용하여 효과 제거
        UItemEffectStrategy* EffectStrategy = CreateEffectStrategy(m_PerkData.ItemEffectType);
        if (EffectStrategy)
        {
            EffectStrategy->RemoveEffect(m_PerkData, World);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ItemPerk: No effect strategy found for type %d"), 
                   (int32)m_PerkData.ItemEffectType);
        }
    }
}

UStatEffectStrategy* UStatPerk::CreateStatEffectStrategy(EStatEffectType EffectType)
{
    switch (EffectType)
    {
        case EStatEffectType::PlayerStat:
            return NewObject<UPlayerStatEffect>(this);
            
        case EStatEffectType::AllyStat:
            return NewObject<UAllyStatEffect>(this);
            
        case EStatEffectType::EnemyStat:
            return NewObject<UEnemyStatEffect>(this);
            
        case EStatEffectType::Custom:
            // TODO: Custom 스탯 효과 구현
            UE_LOG(LogTemp, Warning, TEXT("Custom stat effect not implemented yet"));
            return nullptr;
            
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unknown stat effect type: %d"), (int32)EffectType);
            return nullptr;
    }
}

UItemEffectStrategy* UItemPerk::CreateEffectStrategy(EItemEffectType EffectType)
{
    switch (EffectType)
    {
        case EItemEffectType::GoldIncrease:
            return NewObject<UGoldIncreaseEffect>(this);
            
        case EItemEffectType::ItemQuantity:
            return NewObject<UItemQuantityEffect>(this);
            
        case EItemEffectType::StatBoost:
            // TODO: StatBoost 효과 구현
            UE_LOG(LogTemp, Warning, TEXT("StatBoost effect not implemented yet"));
            return nullptr;
            
        case EItemEffectType::Custom:
            // TODO: Custom 효과 구현
            UE_LOG(LogTemp, Warning, TEXT("Custom effect not implemented yet"));
            return nullptr;
            
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unknown effect type: %d"), (int32)EffectType);
            return nullptr;
    }
}
