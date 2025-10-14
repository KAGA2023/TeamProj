// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "../Status/StatusComponent.h"
#include "PerkStruct.generated.h"

UENUM(BlueprintType)
enum class EPerkType : uint8
{
    None            UMETA(DisplayName = "None"),
    Summon          UMETA(DisplayName = "Summon"),
    Stat            UMETA(DisplayName = "Stat"),
    Buff            UMETA(DisplayName = "Buff"),
    Item            UMETA(DisplayName = "Item")
};

UENUM(BlueprintType)
enum class EItemEffectType : uint8
{
    None            UMETA(DisplayName = "None"),
    GoldIncrease    UMETA(DisplayName = "Gold Increase"),
    ItemQuantity    UMETA(DisplayName = "Item Quantity"),
    StatBoost       UMETA(DisplayName = "Stat Boost"),
    Custom          UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EStatEffectType : uint8
{
    None            UMETA(DisplayName = "None"),
    PlayerStat      UMETA(DisplayName = "Player Stat"),
    AllyStat        UMETA(DisplayName = "Ally Stat"),
    EnemyStat       UMETA(DisplayName = "Enemy Stat"),
    Custom          UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EPerkRarity : uint8
{
    Common          UMETA(DisplayName = "Common"),
    Rare            UMETA(DisplayName = "Rare"),
    Epic            UMETA(DisplayName = "Epic"),
    Legendary       UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FPerkData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName PerkID = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PerkName = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description = TEXT("");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPerkRarity Rarity = EPerkRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPerkType PerkType = EPerkType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Value = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemEffectType ItemEffectType = EItemEffectType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStatEffectType StatEffectType = EStatEffectType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HPBoost = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackBoost = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefenseBoost = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpeedBoost = 0.0f;

    // 특정 유닛 타겟팅 관련 필드들
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bTargetSpecificUnit = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TargetUnitID = NAME_None;

    bool operator==(const FPerkData& Other) const
    {
        return PerkID == Other.PerkID &&
               PerkName == Other.PerkName &&
               Description == Other.Description &&
               Icon == Other.Icon &&
               Rarity == Other.Rarity &&
               PerkType == Other.PerkType &&
               Value == Other.Value &&
               ItemID == Other.ItemID &&
               ItemEffectType == Other.ItemEffectType &&
               StatEffectType == Other.StatEffectType &&
               HPBoost == Other.HPBoost &&
               AttackBoost == Other.AttackBoost &&
               DefenseBoost == Other.DefenseBoost &&
               SpeedBoost == Other.SpeedBoost &&
               bTargetSpecificUnit == Other.bTargetSpecificUnit &&
               TargetUnitID == Other.TargetUnitID;
    }
};

// 특정 유닛 타겟팅을 위한 구조체
USTRUCT(BlueprintType)
struct FTargetedUnitModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName UnitID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FStatModifier Modifier;

    FTargetedUnitModifier()
    {
        UnitID = NAME_None;
    }

    bool operator==(const FTargetedUnitModifier& Other) const
    {
        return UnitID == Other.UnitID && Modifier == Other.Modifier;
    }
};

///////////////////////////Object//////////////////////////////

// Item 효과 전략 인터페이스
UCLASS(Blueprintable, Abstract)
class TEAMPROJ_API UItemEffectStrategy : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool ApplyEffect(const FPerkData& PerkData, UWorld* World);
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool RemoveEffect(const FPerkData& PerkData, UWorld* World);
};

// Gold 증가 효과 전략
UCLASS(Blueprintable)
class TEAMPROJ_API UGoldIncreaseEffect : public UItemEffectStrategy
{
    GENERATED_BODY()

public:
    virtual bool ApplyEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
    virtual bool RemoveEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
};

// 아이템 수량 증가 효과 전략
UCLASS(Blueprintable)
class TEAMPROJ_API UItemQuantityEffect : public UItemEffectStrategy
{
    GENERATED_BODY()

public:
    virtual bool ApplyEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
    virtual bool RemoveEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
};

// 스탯 효과 전략 인터페이스
UCLASS(Blueprintable, Abstract)
class TEAMPROJ_API UStatEffectStrategy : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool ApplyStatEffect(const FPerkData& PerkData, UWorld* World);
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool RemoveStatEffect(const FPerkData& PerkData, UWorld* World);
};

// 플레이어 스탯 증가 효과 전략
UCLASS(Blueprintable)
class TEAMPROJ_API UPlayerStatEffect : public UStatEffectStrategy
{
    GENERATED_BODY()

public:
    virtual bool ApplyStatEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
    virtual bool RemoveStatEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
};

// 아군 스탯 증가 효과 전략
UCLASS(Blueprintable)
class TEAMPROJ_API UAllyStatEffect : public UStatEffectStrategy
{
    GENERATED_BODY()

private:
    // 이미 적용된 아군 캐릭터들을 캐시 (성능 최적화)
    UPROPERTY()
    TArray<class UStatusComponent*> m_AppliedAllies;
    
    // 적용된 스탯 모디파이어 저장
    FStatModifier m_AppliedModifier;

public:
    virtual bool ApplyStatEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
    virtual bool RemoveStatEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
    
    // 새 아군이 소환될 때만 개별 적용 (PerkComponent에서 호출)
    UFUNCTION(BlueprintCallable)
    void ApplyToNewAlly(class UStatusComponent* NewAllyStatusComp);
    
    // 특정 아군에게 적용된 효과 제거
    UFUNCTION(BlueprintCallable)
    void RemoveFromAlly(class UStatusComponent* AllyStatusComp);
};

// 적군 스탯 증가 효과 전략
UCLASS(Blueprintable)
class TEAMPROJ_API UEnemyStatEffect : public UStatEffectStrategy
{
    GENERATED_BODY()

public:
    virtual bool ApplyStatEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
    virtual bool RemoveStatEffect_Implementation(const FPerkData& PerkData, UWorld* World) override;
};

// 기본 퍼크 클래스 (추상 클래스)
UCLASS(Blueprintable, Abstract)
class TEAMPROJ_API UPerk : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString m_PerkName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FPerkData m_PerkData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool m_bIsActive = false;

public:
    UPerk();

    UFUNCTION(BlueprintCallable)
    FPerkData GetPerkData() const { return m_PerkData; }

    UFUNCTION(BlueprintCallable)
    FString GetPerkName() const { return m_PerkName; }

    UFUNCTION(BlueprintCallable)
    bool IsActive() const { return m_bIsActive; }

    UFUNCTION(BlueprintCallable)
    void SetPerkData(const FPerkData& pData);

    UFUNCTION(BlueprintCallable)
    void ActivatePerk();

    UFUNCTION(BlueprintCallable)
    void DeactivatePerk();

    UFUNCTION(BlueprintCallable)
    void TogglePerk();

    // 타입별 특별한 활성화/비활성화 로직을 위한 가상 함수
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void OnPerkActivated();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void OnPerkDeactivated();
};

// 소환 퍼크 클래스
UCLASS(Blueprintable)
class TEAMPROJ_API USummonPerk : public UPerk
{
    GENERATED_BODY()

public:
    USummonPerk();

    virtual void OnPerkActivated_Implementation() override;
    virtual void OnPerkDeactivated_Implementation() override;
};

// 스텟 퍼크 클래스
UCLASS(Blueprintable)
class TEAMPROJ_API UStatPerk : public UPerk
{
    GENERATED_BODY()

public:
    UStatPerk();

    virtual void OnPerkActivated_Implementation() override;
    virtual void OnPerkDeactivated_Implementation() override;

    // 스탯 효과 전략 팩토리 함수
    UFUNCTION()
    UStatEffectStrategy* CreateStatEffectStrategy(EStatEffectType EffectType);
};

// 버프 퍼크 클래스
UCLASS(Blueprintable)
class TEAMPROJ_API UBuffPerk : public UPerk
{
    GENERATED_BODY()

public:
    UBuffPerk();

    virtual void OnPerkActivated_Implementation() override;
    virtual void OnPerkDeactivated_Implementation() override;
};

// 아이템 퍼크 클래스
UCLASS(Blueprintable)
class TEAMPROJ_API UItemPerk : public UPerk
{
    GENERATED_BODY()

public:
    UItemPerk();

    virtual void OnPerkActivated_Implementation() override;
    virtual void OnPerkDeactivated_Implementation() override;

private:
    // 효과 전략 팩토리 함수
    UFUNCTION()
    UItemEffectStrategy* CreateEffectStrategy(EItemEffectType EffectType);
};
