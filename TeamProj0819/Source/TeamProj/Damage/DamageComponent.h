// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Skill/SkillStruct.h"
#include "DamageComponent.generated.h"


USTRUCT(BlueprintType)
struct FSkillTraceParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 120.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HalfHeight = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConeHalfAngle = 45.f; //전방 콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> Channel = ECC_Visibility; //적으로 트레이스 바꿔주면됨 임시로 설정해뜸
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* OptionalTarget = nullptr;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJ_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	bool b_Stunned = false;			//스턴
	bool b_Invincible = false;		//무적
	bool b_Provoke = false;			//도발
	bool b_Provoked = false;		//도발당함
	bool b_SuperArmor = false;		//강직(면역)

	FTimerHandle Timer_Stun;
	FTimerHandle Timer_Invincible;
	FTimerHandle Timer_Provoke;
	FTimerHandle Timer_Provoked;
	FTimerHandle Timer_SuperArmor;


	void ApplyStun(float Duration);
	void ClearStun();
	void ApplyInvincible(float Duration);
	void ClearInvincible();
	void ApplyProvoke(float Duration);
	void ClearProvoke();
	void ApplyProvoked(float Duration);
	void ClearProvoked();
	void ApplySuperArmor(float Duration);
	void ClearSuperArmor();
	
	void ApplyKnockBack(float Distance, const FVector& Dir);
	void ApplyKnockFront(float Distance, AActor* Source);

protected:
	TArray<FActiveBuff> ActiveBuffs;					//디버프
	TArray<FActiveStatusEffect> ActiveStatusEffects;	//상태이상


public:	
	UDamageComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



protected:
	virtual void BeginPlay() override;

public:	
	// CC & StatusEffect
	void CalculateStatusEffect(const FSkillLevelData& statusEffectType);
	void CalculateCC(const FSkillLevelData& CCType);

	void AddBuff(const FSkillLevelData& pBuff);
	void RemoveBuff(const FSkillLevelData& pBuff);
	void AddStatusEffect(const FSkillEffectInfo& pStatusEffect);
	void HandleExpiredStatusEffect(const FActiveStatusEffect& ExpiredEffect);
	void ApplyDotDamage(const FActiveStatusEffect& Effect);

		
};
