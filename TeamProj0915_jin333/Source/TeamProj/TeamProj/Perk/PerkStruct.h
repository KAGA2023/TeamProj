// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
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

    bool operator==(const FPerkData& Other) const
    {
        return PerkID == Other.PerkID &&
               PerkName == Other.PerkName &&
               Description == Other.Description &&
               Icon == Other.Icon &&
               Rarity == Other.Rarity &&
               PerkType == Other.PerkType;
    }
};

///////////////////////////Object//////////////////////////////

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
};
