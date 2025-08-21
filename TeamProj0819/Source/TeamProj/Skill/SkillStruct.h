// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Status/StatusStruct.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "UObject/NoExportTypes.h"
#include "SkillStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSkillConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ClassType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count;
};


UENUM(BlueprintType)
enum class ESkillType : uint8
{
    None             UMETA(DistplayName = "None"),
    Attack           UMETA(DistplayName = "Attack"),
    Buff             UMETA(DistplayName = "Buff"),
    DeBuff           UMETA(DistplayName = "DeBuff"),
    Heal             UMETA(DistplayName = "Heal")
};

UENUM(BlueprintType)
enum class ESkillTriggerType : uint8
{
    None             UMETA(DistplayName = "None"),          //None
    One              UMETA(DistplayName = "Defealt"),       //단일 타겟, 기본공격 X
    Self             UMETA(DistplayName = "Self"),          //자신 X
    SelfRadius       UMETA(DistplayName = "SelfRadius"),    //플레이어주변범위 X
    TargetRadius     UMETA(DistplayName = "TargetRadius"),  //주변 타겟위치내 범위 
    TargetPoint      UMETA(DistplayName = "TargetPoint"),   //타겟 지정 설정 O
};

UENUM(BlueprintType)
enum class ESkillEffectType : uint8
{
    None             UMETA(DistplayName = "None"),
    Hurt             UMETA(DistplayName = "Hurt"),
    Poison           UMETA(DistplayName = "Posion"),
    Fire             UMETA(DistplayName = "Fire"),
    Iced             UMETA(DistplayName = "Iced"),
    TickHeal         UMETA(DistplayName = "TickHeal")
};

UENUM(BlueprintType)
enum class ESkillCCType : uint8
{
    None             UMETA(DistplayName = "None"),
    Stun             UMETA(DistplayName = "Stun"),
    Invincible       UMETA(DistplayName = "Invincible"),
    KnockBack        UMETA(DistplayName = "KnockBack"),
    KnockFront       UMETA(DistplayName = "KnockFront"),
    Provoke          UMETA(DistplayName = "Provoke"),
    SuperArmor       UMETA(DistplayName = "SuperArmor")
};




USTRUCT(BlueprintType)
struct FSkillEffectInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName            SkillEffectName = TEXT("");
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESkillEffectType EffectType = ESkillEffectType::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32            MaxStack = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            Duration = 0.f;
    
    bool operator==(const FSkillEffectInfo& Other) const
    {
        return SkillEffectName == Other.SkillEffectName &&
            FMath::IsNearlyEqual(Duration, Other.Duration) &&
            MaxStack == Other.MaxStack &&
            EffectType == Other.EffectType;
    }

};


USTRUCT(BlueprintType)
struct FSkillCCInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName            SkillCCName = TEXT("");
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESkillCCType     EffectType = ESkillCCType::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32            MaxStack = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            Duration = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            KnockDistance = 0.f;
    
    
    bool operator==(const FSkillCCInfo& Other) const
    {
        return SkillCCName == Other.SkillCCName &&
            FMath::IsNearlyEqual(Duration, Other.Duration) &&
            FMath::IsNearlyEqual(KnockDistance, Other.KnockDistance) &&
            MaxStack == Other.MaxStack &&
            EffectType == Other.EffectType;
    }

};

USTRUCT(BlueprintType)
struct FSkillLevelData : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            AddDamage = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            CoolRec = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            AddDuration = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            CastingTimeRec = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            AddHalfHeight = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            AddRange = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            AddRadius = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float            AddConeHalfAngle = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FStatModifier    BuffStat;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FSkillEffectInfo ExtraEffect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FSkillCCInfo     ExtraCC;
    
    bool operator==(const FSkillLevelData& Other) const
    {
        return 
            FMath::IsNearlyEqual(AddDamage, Other.AddDamage) &&
            FMath::IsNearlyEqual(CoolRec, Other.CoolRec) &&
            FMath::IsNearlyEqual(AddDuration, Other.AddDuration) &&
            FMath::IsNearlyEqual(CastingTimeRec, Other.CastingTimeRec) &&
            FMath::IsNearlyEqual(AddHalfHeight, Other.AddHalfHeight) &&
            FMath::IsNearlyEqual(AddRadius, Other.AddRadius) &&
            FMath::IsNearlyEqual(AddConeHalfAngle, Other.AddConeHalfAngle) &&
            BuffStat == Other.BuffStat &&
            ExtraEffect == Other.ExtraEffect &&
            ExtraCC == Other.ExtraCC;
    }
};

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)     FName                   SkillID = TEXT("");
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     FString                 SkillName = TEXT("");
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     FString                 Description = TEXT("");
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     UTexture2D*             Icon = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     UAnimMontage*           Animation = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     UNiagaraSystem*         Effect;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     USoundBase*             Sound = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     ESkillType              SkillType = ESkillType::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     ESkillTriggerType       SkillTrigger = ESkillTriggerType::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     FName                   WeaponType = TEXT("");
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     EClassType              TargetType = EClassType::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     float                   HalfHeight = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     float                   Range = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     float                   Radius = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     float                   ConeHalfAngle = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     float                   Cooldown = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     float                   CastingTime = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     float                   Duration = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)     TArray<FSkillLevelData> SkillData;
};


/////////////////////////////CC////////////////////////////

USTRUCT(BlueprintType)
struct FActiveBuff
{
    GENERATED_BODY()

    UPROPERTY()
    FSkillLevelData BuffData;
    //나중에 아이콘도 고려 -> UI로 보드캐스팅 고고
    UPROPERTY()
    float RemainingDuration;


    FActiveBuff() : RemainingDuration(0.f)
    {}

    FActiveBuff(const FSkillLevelData& InBuffData, float InDuration) : BuffData(InBuffData), RemainingDuration(InDuration)
    {}
};



USTRUCT(BlueprintType)
struct FActiveStatusEffect
{
    GENERATED_BODY()

    UPROPERTY()
    FSkillEffectInfo EffectData;

    UPROPERTY()
    float RemainingDuration;

    UPROPERTY()
    int32 CurrentStack;

    UPROPERTY()
    float TickDmgTime;

    FActiveStatusEffect()
        : RemainingDuration(0.f), CurrentStack(1), TickDmgTime(0.f)
    {}

    FActiveStatusEffect(const FSkillEffectInfo& InEffectData, float InDuration)
        : EffectData(InEffectData), RemainingDuration(InDuration), CurrentStack(1), TickDmgTime(0.f)
    {}
};




///////////////////////////Object//////////////////////////////



UCLASS(Blueprintable)
class TEAMPROJ_API USkill : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FString m_SkillName;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    int32 m_CurrentLevel;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FSkillData m_SkillData;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    float m_Duration;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    float m_CoolRemaining;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    float m_CastingTime;
    

public:
    USkill();
    UFUNCTION(BlueprintCallable)
    int32 GetSkillLevel() { return m_CurrentLevel; }
    UFUNCTION(BlueprintCallable)
    FSkillData GetSkillData() const { return m_SkillData; }
    UFUNCTION(BlueprintCallable)
    FSkillLevelData GetSkillLevelData() const;

    UFUNCTION(BlueprintCallable)
    void SetSkillData(const FSkillData& pData);
    UFUNCTION(BlueprintCallable)
    void SetSkillLevel(int32 nLevel);

    UFUNCTION(BlueprintCallable)
    bool SkillLevelUp();
    UFUNCTION(BlueprintCallable)
    bool ExecuteSkill(AActor *Source);
    
    UFUNCTION(BlueprintCallable)
    bool CoolStart();
    UFUNCTION(BlueprintCallable)
    bool CoolDown(float DeltaTime);
    UFUNCTION(BlueprintCallable)
    bool IsReady() const { return m_CoolRemaining<=0.f; }
};
